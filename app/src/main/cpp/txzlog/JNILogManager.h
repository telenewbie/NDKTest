#ifndef __TXZ_JNI_LOG_MANAGER_H__
#define __TXZ_JNI_LOG_MANAGER_H__

#include <deque>
#include <map>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>

class JNILogManager
{
public:
	typedef struct Logmsg
	{
		long pid;
		long tid;
		int  logid;
		std::string func_name;
		std::string file_name;
		int  line;
		int  level;
		std::string tag;
		std::string msg;
		time_t time;
		time_t clock;
		char szTime[30];
	}Logmsg;
	

	enum
	{
	    OSAL_LOG_DEBUG = 3, //
	    OSAL_LOG_INFO = 4, //
	    OSAL_LOG_WARN = 5, //
	    OSAL_LOG_ERROR = 6, //
	    OSAL_LOG_FATAL = 7 //
	};

    //保护构造，禁止外部直接创建实例
    JNILogManager(int default_level);
protected:
    ~JNILogManager();
    
    std::deque< JNILogManager::Logmsg* >  m_queLogMessage;
    char m_szErrMsg[256];
    FILE* m_pfile;
    size_t m_nCurPos;
    pthread_t m_pthLogThread; //
	pthread_cond_t m_cndLogThread; //
    pthread_mutex_t m_mtxLogMessage; //
    time_t m_nLastWriteTime;
    size_t m_nLostCount = 0;
    int m_default_level = 0;
    size_t m_nLogFileSize;
    size_t m_nLogFileCnt;
    std::string m_sFile0Path;
	std::string m_sFilePath;

    
    
	
public:
	static void* _ProcLogMessages(void* mgr);
	void ProcLogMessages();
	bool isEnableLevel(int level);

	void writeLogMessage(long pid,
						long tid,
						int  logid,
						std::string func_name,
						std::string file_name,
						int  line,
						int  level,
						std::string tag,
						std::string msg);

	const char* getLevel(
            const int iLevel)
    {
        static char arrMsg[][10] = { "UNKNOWN ", "DEFAULT", "VERBOSE", "D", "I", "W", "E", "F", "SILENT" };
        if (iLevel >= 9) return arrMsg[0];
        return arrMsg[iLevel];
    };

    bool log_init();
    bool log_write(JNILogManager::Logmsg& pbLog);
    bool shiftFiles();
    bool writeFile(const void* data, size_t len);
    bool reopenFile();
    void closefile();
    inline void getLogFileName(
            char* buf,
            size_t bufsize,
            int index);

	inline int64_t GetTickCount()
	{
	    struct timespec ts;
	    clock_gettime(CLOCK_MONOTONIC, &ts);
	    return (((int64_t) ts.tv_sec) * 1000 + ts.tv_nsec / 1000000);		
	}

static inline uint64_t mtime()//单位：毫秒
    {
        struct timeval tv;
        gettimeofday((&tv), NULL);
        uint64_t nTime = tv.tv_sec;
        nTime *= 1000;
        nTime += tv.tv_usec / 1000;
        return nTime*1000;
    }
	
};



#endif
