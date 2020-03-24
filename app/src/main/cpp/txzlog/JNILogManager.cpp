#include<unistd.h>
#include <android/log.h>
#include "JNILogManager.h"

#define MAX_LOG_IN_QUEUE (1000)    //队列中最多保存多少条日志，防止内存无限增长
#define MIN_LOG_TO_WRITE  (20)        //队列超过多少条日志落地一次
#define MAX_TIME_WAIT_WRITE  (3000) //最多3秒落地一次日志,不设置为无穷大
#define LOG_FILE_SIZE (0)            //日志文件大小
#define LOG_FILE_MAX_CNT (0)        //日志文件个数
//#define LOG_FILE_PATH ("./log")		//日志文件个数
#define LOG_FILE_PATH ("/sdcard/txz")        //日志文件个数
#define LOG_FILE_NAME ("test_all")  //日志文件名

//使用单例构造获取，在全局中构造单例
// JNILogManager* JNILogManager::m_pobjModule = &(JNILogManager::getInstance());

// JNILogManager& JNILogManager::getInstance()
// {
//     static CLogManager objModule;
//     return objModule;
// }

//构造
JNILogManager::JNILogManager(int default_level) {
    m_default_level = default_level;
    m_pfile = NULL;
    m_nCurPos = 0;

    m_nLastWriteTime = 0;
    m_nLostCount = 0;

    m_nLogFileSize = LOG_FILE_SIZE;
    m_nLogFileCnt = LOG_FILE_MAX_CNT;

    pthread_mutex_init(&m_mtxLogMessage, NULL);
    pthread_cond_init(&m_cndLogThread, NULL);

    log_init();
}

//析构
JNILogManager::~JNILogManager() {
    pthread_cond_destroy(&m_cndLogThread);
    pthread_mutex_destroy(&m_mtxLogMessage);
}

bool JNILogManager::log_init() {
    std::string sPath = LOG_FILE_PATH;
    if (m_nLogFileSize == 0) {//默认5M
        m_nLogFileSize = 5 * 1024 * 1024;
    } else if (m_nLogFileSize < 1024 * 1024) {
        m_nLogFileSize = 1024 * 1024;
    } else if (m_nLogFileSize > 10 * 1024 * 1024) {
        m_nLogFileSize = 10 * 1024 * 1024;
    }

    if (m_nLogFileCnt == 0) {//默认10个
        m_nLogFileCnt = 10;
    } else if (m_nLogFileCnt < 2) {
        m_nLogFileCnt = 2;
    } else if (m_nLogFileCnt > 20) {
        m_nLogFileCnt = 20;
    }

    m_sFilePath = sPath;
    char szFile[1024];
    snprintf(szFile, sizeof(szFile), "%s/%s", sPath.c_str(), LOG_FILE_NAME);
    m_sFile0Path = szFile;

    //启用线程
    pthread_create(&m_pthLogThread, NULL, _ProcLogMessages, this);

    return true;
}

void *JNILogManager::_ProcLogMessages(
        void *mgr) {
    JNILogManager *p = (JNILogManager *) mgr;
    p->ProcLogMessages();
    return NULL;
}

void JNILogManager::ProcLogMessages() {
    prctl(PR_SET_NAME, "LogManager");//线程常用名
    setpriority(PRIO_PROCESS, 0, -1);//优先级调低
    __android_log_print(ANDROID_LOG_DEBUG, "native:newbie:", "%s:%d", __FUNCTION__, __LINE__);
    pthread_mutex_lock(&m_mtxLogMessage);

    while (true) {
#if MAX_TIME_WAIT_WRITE > 0
        if (m_queLogMessage.empty()) {
            struct timeval now;
            struct timespec outtime;
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec + MAX_TIME_WAIT_WRITE / 1000;
            outtime.tv_nsec = now.tv_usec * 1000 + MAX_TIME_WAIT_WRITE % 1000;
            pthread_cond_timedwait(&m_cndLogThread, &m_mtxLogMessage, &outtime);
        }
#else
        if (m_queLogMessage.empty())
        {
            pthread_cond_wait(&m_cndLogThread, &m_mtxLogMessage);
        }
#endif
        if (!m_queLogMessage.empty()) {
            //先记录下现场，再释放锁，提升日志性能
            size_t lostcount = m_nLostCount;
            m_nLostCount = 0;
            std::deque<JNILogManager::Logmsg *> ques;
            ques.swap(m_queLogMessage);

            {
                pthread_mutex_unlock(&m_mtxLogMessage);

                if (lostcount > 0 && isEnableLevel(OSAL_LOG_WARN)) {
                    //__android_log_print(ANDROID_LOG_ERROR, "[CLogManager]", "errmsg=lost %u log records", (uint32_t)lostcount);
                    JNILogManager::Logmsg logmsg;
                    logmsg.pid = 0;
                    logmsg.tid = 0;
                    logmsg.logid = 0;
                    logmsg.func_name = "LOG";
                    logmsg.file_name = "LOG";
                    logmsg.line = 0;
                    logmsg.level = OSAL_LOG_WARN;
                    logmsg.tag = "LOG";
                    char buf[40] = {0};
                    snprintf(buf, 40, "lost %d log records", lostcount);
                    logmsg.msg = buf;
                    logmsg.time = mtime();
                    logmsg.clock = GetTickCount();

                    time_t nTime = logmsg.time / 1000000;
                    struct tm curr = *localtime(&nTime);
                    snprintf(logmsg.szTime, sizeof(logmsg.szTime), "%04d-%02d-%02d %02d:%02d:%02d",
                             curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday, curr.tm_hour,
                             curr.tm_min, curr.tm_sec);
                    log_write(logmsg);
                } else {
                    __android_log_print(ANDROID_LOG_DEBUG, "native:newbie:",
                                        "%s:%d,lostCount=%d,supportlevel=%d,nowLevel=%d",
                                        __FUNCTION__, __LINE__,
                                        m_default_level, OSAL_LOG_WARN);
                }

                for (auto it = ques.begin(); it != ques.end(); ++it) {
                    JNILogManager::Logmsg *msg = *it;
                    log_write(*msg);
                    delete msg;
                }

                pthread_mutex_lock(&m_mtxLogMessage);
            }
        }
    }

    pthread_mutex_unlock(&m_mtxLogMessage);
}

bool JNILogManager::log_write(
        JNILogManager::Logmsg &pbLog) {
    __android_log_print(ANDROID_LOG_DEBUG, "native:newbie:",
                        "%s:%d,pbLog.level=%d,m_default_level=%d", __FUNCTION__, __LINE__,
                        pbLog.level, m_default_level);
    static time_t s_nFailTime = 0;
    if (1/*s_nFailTime < CServerTime::time() + 60*/) {//60s内没有失败才写日志，为了解决sdcard挂载慢的问题
        if (pbLog.level >= m_default_level) {//是否关闭text日志
            //写txt文件
            static char *szWriteData = new char[1024];
            static int nWriteDataLen = 1024;
            int need =
                    ((pbLog.msg.size() + pbLog.func_name.size() + 256/*打印附加信息预留*/+ 1023) / 1024) *
                    1024;
            if (nWriteDataLen < need) {
                delete[] szWriteData;
                szWriteData = new char[nWriteDataLen = need];
            }
            int n = snprintf(szWriteData, nWriteDataLen,
                             "[%06u"  "][%s][%s.%03u"  "][%d/%u"  "][%u"  "][%s][%s] %s\n", //格式化文本
                             pbLog.logid, //用户UUID
                             getLevel(pbLog.level), //日志等级
                             pbLog.szTime, (pbLog.time % 1000000) / 1000, //时间
                             pbLog.pid, //进程号
                             pbLog.tid, //线程号
                             pbLog.clock, //启动经过的时间
                             (pbLog.file_name + "::" +
                              pbLog.func_name).c_str(), //包名后缀，去掉com.txznet.，最多输出6个字符，以防第三方包名错误
                             pbLog.tag.c_str(), //日志标签
                             pbLog.msg.c_str() //日志内容
            );
            __android_log_print(ANDROID_LOG_DEBUG, "native:newbie:", "%s:%d，%s", __FUNCTION__,
                                __LINE__, szWriteData);
            if (n >= nWriteDataLen) {
                n = nWriteDataLen;
            } else if (n < 0) {
                n = 0;
            }
            if (!writeFile(szWriteData, n)) {
                s_nFailTime = 0;//CServerTime::time();
                __android_log_print(ANDROID_LOG_ERROR, "[CLogManager]", "errmsg=%s",
                                    "happend error");
            }
        }
    }
    return true;
}

bool JNILogManager::isEnableLevel(int level) {
    return level >= m_default_level && level < 8;
}

void JNILogManager::writeLogMessage(long pid,
                                    long tid,
                                    int logid,
                                    std::string func_name,
                                    std::string file_name,
                                    int line,
                                    int level,
                                    std::string tag,
                                    std::string msg) {
    if (!isEnableLevel(level)) {
        return;
    }

    {
        JNILogManager::Logmsg *logmsg = new JNILogManager::Logmsg();

        pthread_mutex_lock(&m_mtxLogMessage);
        logmsg->pid = pid;
        logmsg->tid = tid;
        logmsg->logid = logid;
        logmsg->func_name = func_name;
        logmsg->file_name = file_name;
        logmsg->line = line;
        logmsg->level = level;
        logmsg->tag = tag;
        logmsg->msg = msg;

        logmsg->time = mtime();
        // printf("time:%u\n", logmsg->time);
        logmsg->clock = GetTickCount();
        // printf("clock:%u\n", logmsg->clock);

        time_t nTime = logmsg->time / 1000000;
        struct tm curr = *localtime(&nTime);
        snprintf(logmsg->szTime, sizeof(logmsg->szTime), "%04d-%02d-%02d %02d:%02d:%02d",
                 curr.tm_year + 1900, curr.tm_mon + 1, curr.tm_mday, curr.tm_hour,
                 curr.tm_min, curr.tm_sec);
        // printf("szTime:%s\n", logmsg->szTime);

        m_queLogMessage.push_back(logmsg);
        if (m_queLogMessage.size() > MAX_LOG_IN_QUEUE) {
            delete m_queLogMessage[0];
            m_queLogMessage.pop_front();
            ++m_nLostCount;
        }
        //每累计MIN_LOG_TO_WRITE条日志才去通知处理线程处理一下
#if MIN_LOG_TO_WRITE > 1
        if (m_queLogMessage.size() >= MIN_LOG_TO_WRITE)
#endif
        {
            pthread_cond_signal(&m_cndLogThread);
        }

        pthread_mutex_unlock(&m_mtxLogMessage);

        return;
    }

    return;
}

inline void JNILogManager::getLogFileName(
        char *buf,
        size_t bufsize,
        int index) {
    if (index == 0)
        snprintf(buf, bufsize, "%s", m_sFile0Path.c_str());
    else
        snprintf(buf, bufsize, "%s_%d", m_sFile0Path.c_str(), index);
}

bool JNILogManager::shiftFiles() {
    closefile();

    const int bufsize = 1024;
    char buf1[bufsize], buf2[bufsize];
    char *next = buf1, *prev = buf2;

    int i = m_nLogFileCnt - 1;

    getLogFileName(next, bufsize, i);
    if (remove(next) < 0) //运行一段时间后满足的概率更大
    {
        if (access(next, 0) == 0) {
            //snprintf(m_szErrMsg, sizeof(m_szErrMsg), "remove error: errno=%d.", errno);
            __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "rename error:errno=%s,%d",
                                __FUNCTION__, __LINE__
            );
            return false;
        }
    }

    for (--i; i >= 0; --i) {
        getLogFileName(prev, bufsize, i);
        if (rename(prev, next) < 0) //运行一段时间后满足的概率更大
        {
            if (access(prev, 0) == 0) {
                __android_log_print(ANDROID_LOG_DEBUG, "native:newbie",
                                    "rename error:errno=%s,%d",
                                    __FUNCTION__, __LINE__
                );
//                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "rename error:errno=%d.", errno);
                return false;
            }
        }
        char *tmp = prev;
        prev = next;
        next = tmp;
    }

    return true;
}

bool JNILogManager::writeFile(
        const void *data,
        size_t len) //不需要trace
{
    __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "%s,%d,len= %d",
                        __FUNCTION__, __LINE__, len);
    if (len <= 0) return true;

    if (m_nCurPos >= m_nLogFileSize) {
        if (!shiftFiles()) {
            __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "rename error:errno=%s,%d",
                                __FUNCTION__, __LINE__);
            return false;
        }
    }

    if (NULL == m_pfile) {

        if (!reopenFile()) {
            __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "rename error:errno=%s,%d",
                                __FUNCTION__, __LINE__);
            return false;
        }
    }

    if (fwrite(data, 1, len, m_pfile) != len) {
        //snprintf(m_szErrMsg, sizeof(m_szErrMsg), "fwrite file(%s) error:%s.", m_sFile0Path.c_str(), strerror(errno));
        __android_log_print(ANDROID_LOG_DEBUG, "native:newbie",
                            "rename error:errno=%s,%d,File=%s",
                            __FUNCTION__, __LINE__, m_sFile0Path.c_str());
        closefile();
        return false;
    }
    __android_log_print(ANDROID_LOG_DEBUG, "native:newbie",
                        "%s,%d write success %s",
                        __FUNCTION__, __LINE__,m_sFile0Path.c_str());
    //fflush(m_pfile);
    m_nCurPos += len;

    return true;
}

bool JNILogManager::reopenFile() {
    closefile();

    if (access(m_sFilePath.c_str(), F_OK) != 0)
        if (mkdir(m_sFilePath.c_str(), 0777) < 0) {
            // if (EEXIST != errno)
            // {
            __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "%s,%d,filename=%s",
                                __FUNCTION__,
                                __LINE__, m_sFilePath.c_str());
            //snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init error: create path<%s> error: %s", m_sFilePath.c_str(), strerror(errno));
            return false;
            // }
        }

    m_pfile = fopen(m_sFile0Path.c_str(), "ab+");
    if (NULL == m_pfile) {
        __android_log_print(ANDROID_LOG_DEBUG, "native:newbie", "%s,%d,filename=%s", __FUNCTION__,
                            __LINE__, "file ptr is null");
        //snprintf(m_szErrMsg, sizeof(m_szErrMsg), "fopen file(%s) error:%s.", m_sFile0Path.c_str(), strerror(errno));
        return false;
    }
    fseek(m_pfile, 0, SEEK_END);
    m_nCurPos = ftell(m_pfile);

    return true;
}

void JNILogManager::closefile() {
    if (NULL != m_pfile) {
        fclose(m_pfile);
        m_pfile = NULL;
    }
    m_nCurPos = 0;
}


