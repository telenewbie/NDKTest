package com.txz.engine;

import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

import com.example.utils.AudioTrackUtils;
import com.example.utils.RecorderUtils;

public abstract class Unisound {

    static {
        System.loadLibrary("txzEngine");
    }

    /**
     * LP:低功耗
     * STD:标准功耗
     * WAKEUP:唤醒
     * LASR:离线识别
     * RASR:在线识别
     */
    public interface UNI_RECOGN_MODE {
        int UNI_LP_WAKEUP = 0;
        int UNI_STD_WAKEUP = 1;
        int UNI_LP_LASR = 2;
        int UNI_STD_LASR = 3;
        int UNI_RASR_MODE = 4;
        int UNI_LP_LASR_RASR = 5;
        int UNI_STD_LASR_RASR = 6;
        int UNI_LP_WAKEUP_LOW = 7;
        int UNI_STD_WAKEUP_LOW = 8;
        int UNI_LP_WAKEUP_RASR = 9;
        int UNI_STD_WAKEUP_RASR = 10;

        int UNI_MODE_MAX = 11;
    }

    /**
     * 初始化
     * return:true 成功，false，失败
     */
    public native static boolean Initialize();

    /**
     * 反始化
     */
    public native static boolean Finalize();

    /**
     * 创建识别
     * configPath:config的路径
     * return:0为成功，非零为错误
     */
    public native static int RecognCreate(byte[] configPath);

    /**
     * 销毁识别
     */
    public native static void RecognDestroy();

    /**
     * 启动识别
     * mode:参考com.txz.engine.Unisound.UNI_RECOGN_MODE
     */
    public native static int RecognStart(int mode);

    /**
     * 停止识别，返回结果
     */
    public native static int RecognStop();

    /**
     * 取消识别，不放回识别结果
     */
    public native static int RecognCancel();

    /**
     * 创建tts
     * configPath:config路径
     */
    public native static int TtsCreate(byte[] configPath);

    /**
     * 销毁tts
     */
    public native static int TtsDestroy();

    /**
     * 打开tts
     * channels:通道数
     * bit:位数
     * frequency:采样率
     */
    public native static int TtsOpen(int channels, int bit, int frequency);

    /**
     * 关闭tts
     */
    public native static int TtsClose();

    /**
     * 播放tts
     * data:播放数据
     */
    public native static int TtsPlay(byte[] data);

    /**
     * 关闭tts
     */
    public native static int TtsStop();


    //*****************************需要实现***********************************


    private static RecorderUtils mRecorder;

    private static int isSuccess(Object obj) {
        return obj != null ? 0 : -1;
    }

    /**
     * 打开 录音
     * audioSource:不支持，保留
     * channels:通道，默认1
     * bit:位数，默认16
     * frequency:采样率，默认16000
     * return:0成功；非0失败
     */
    public static int CaptureOpen(long audioSource, int channels, int bit, int frequency) {
        mRecorder = new RecorderUtils.Builder().setAudioFormat(bit).setChannelConfig(channels).setSampleRateInHz(frequency).build();
        return isSuccess(mRecorder);
    }

    /**
     * 关闭录音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int CaptureClose(long audioSource) {
        mRecorder.closeRecorder();
        return 0;
    }

    /**
     * 开始录音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int CaptureStart(long audioSource) {
        mRecorder.startRecording();
        return 0;
    }

    /**
     * 结束录音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int CaptureStop(long audioSource) {
        mRecorder.stopRecording();
        return 0;
    }

    /**
     * 读录音
     * audioSource:不支持，保留
     * buf:录音数据
     * size:buf的size
     * return:大于等于0成功，返回读取的读取的字节数；小于0失败
     */
    public static int CaptureRead(long audioSource, byte[] buf, int size) {
        return mRecorder.readData(buf, 0, size);
    }

    private static AudioTrackUtils mAudioTrack;

    /**
     * 打开放音，audiotrack
     * audioSource:不支持，保留
     * channels:通道数
     * bit:位数
     * frequency:采样率
     * return:0成功；非0失败
     */
    public static int AudioOpen(long audioSource, int channels, int bit, int frequency) {
        mAudioTrack = new AudioTrackUtils.Builder().setAudioFormat(bit).setSampleRateInHz(frequency).setChannelConfig(channels).build();
        return isSuccess(mAudioTrack);
    }

    /**
     * 关闭 放音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int AudioClose(long audioSource) {
        mAudioTrack.closeAudioTrack();
        return 0;
    }

    /**
     * 开始放音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int AudioStart(long audioSource) {
        mAudioTrack.startPlaying();
        return 0;
    }

    /**
     * 结束放音
     * audioSource:不支持，保留
     * return:0成功；非0失败
     */
    public static int AudioStop(long audioSource) {
        mAudioTrack.stopPlaying();
        return 0;
    }

    /**
     * 放音 中写入数据
     * audioSource:不支持，保留
     * buf:播放的数剧
     * size:buf的大小
     * return:大于0成功，播放字节数；非0失败
     */
    public static int AudioWrite(long audioSource, byte[] buf, int size) {
        return mAudioTrack.writeData(buf, 0, size);
    }


    interface UNI_VOICE_COMMAND {

        //lasr
        int LASR_BASIC_ID = 0;
        int LASR_ENGINE_MODE = 1;
        int LASR_STD_THRESHOLD = 2;
        int LASR_LP_THRESHOLD = 3;
        int LASR_SKIP_FRAM = 4;
        int LASR_ENGINE_LOG_EN = 5;
        int LASR_STD_MODEL_ID = 6;
        int LASR_DOMAIN = 7;
        int LASR_MODEL_PATH = 8;
        int LASR_TIMEOUT = 9;
        int LASR_WAKEUP_WORD = 10;
        int LASR_NEXT_SCENE = 11;
        int LASR_DATABUF_TIMEMS = 12;
        int LASR_SAVE_RECORD = 13;
        int LASR_SAVE_PATH = 14;
        int LASR_MAX_ID = 100;
        //asr
        int RASR_BISIC_ID = 101;
        int ASR_SIL_TIMEOUT = 102;
        int RASR_APP_KEY = 103;
        int RASR_SEC_KEY = 104;
        int RASR_DOMAIN = 105;
        int RASR_TCP_CON_TPYE = 106;
        int RASR_TCP_TRANSLATE = 107;
        int RASR_OWNER_ID = 108;
        int RASR_DEVICE_ID = 109;
        int RASR_TOKEN = 110;
        int RASR_FILTER_URL = 111;
        int RASR_DP_NAME = 112;
        int RASR_ADDITIONAL_SERVICE = 113;
        int RASR_SCENARIO = 114;
        int RASR_SUB_DOMAIN = 115;
        int RASR_VOICE_FIELD = 116;
        int RASR_NLU_SCENARIO = 117;
        int RASR_CHIP_CLIENT = 118;
        int RASR_NET_TIMEOUT = 119;
        int RASR_PROTOCAL = 120;
        int RASR_PASS_THROUGH = 121;
        int RASR_CODEC = 122;
        int RASR_SESSION_ID = 123;
        int RASR_IP = 124;
        int RASR_PORT = 125;
        int RASR_TIMEOUT = 126;
        int RASR_SAVE_FILE = 127;
        int RASR_SAVE_PATH = 128;
        int RASR_DATABUF_LEN_MS = 129;
        int RASR_SELF_RES = 130;
        int RASR_MAX_ID = 200;
        // preproc
        int PREPROC_BASIC_ID = 201;
        int PREPROC_4MICL_EN = 202;
        int PREPROC_4MICL_AEC_EN = 203;
        int PREPROC_2MIC_EN = 204;
        int PREPROC_RESAMPLE_EN = 205;
        int PREPROC_RECOGN_MODE = 206;
        int PREPROC_TIME_LEN_STAMP = 207;
        int PREPROC_SAVE_FILE = 208;
        int PREPROC_SAVE_PATH = 209;
        int PREPROC_MULTIPLY = 210;
        int PREPROC_MIX_ID = 300;
        /*tts*/
        int TTS_BASIC_ID = 301;
        int TTS_HOST = 302;
        int TTS_PORT = 303;
        int TTS_PARAM_PTR = 304;
        int TTS_PROTOCAL = 305;
        int TTS_CODEC = 306;
        int TTS_APP_KEY = 307;
        int TTS_SEC_KEY = 308;
        int TTS_SPEAKER = 309;
        int TTS_SPD = 310;
        int TTS_VOL = 311;
        int TTS_PIT = 312;
        int TTS_EMT = 313;
        int TTS_SMT = 314;
        int TTS_SAVE_FILE = 315;
        int TTS_SAVE_PATH = 316;
        int TTS_MAX_ID = 400;
        //log
        int LOG_BASIC_ID = 401;
        int LOG_LEVEL = 402;
        int LOG_WAY = 403;
        int LOG_FILE = 404;
        int LOG_MAX_ID = 500;
        int SAV_REC_BASIC_ID = 501;
        int SAV_REC_FLG = 502;       /* 1: save record file 0: do not save record file */
        int SAV_ALS_REC_FLG = 503;   /* 1: save analysis record file 0: do not save analysis record file*/
        int SAV_SED_REC_FLG = 504;   /* 1: save send to engine record file 0: do not save send to engine record file*/
        int SAV_REC_PATH = 505;      /* set save record file path */
        int SAV_REC_MAX_ID = 600;
        //vad
        int VAD_BASIC_ID = 601;
        int VAD_ENABLE = 602;
        int VAD_MAXSILLEN = 603;
        int VAD_USE_LOW_VAD = 604;
        int VAD_ST_ENABLE = 605;
        int VAD_END_ENABLE = 606;
        int VAD_SILENCE_TIMEOUT = 607;
        int VAD_VOICE_TIMEOUT = 608;
        int VAD_PRE_TIME_MS = 609;
        int VAD_MAX_ID = 700;
        //         capture
        int CAPTURE_BASIC_ID = 701;
        int CAPTURE_TIMEOUT = 702;
        int CAPTURE_CUR_SESSION_TIMEMS = 703;
        int CAPTURE_MAX_ID = 800;
        int COMMAND_MAX = 1000;
    }

    public interface UNI_EVENT_ENUM {
        /* vad */
        int UNI_VAD_SPEECH_START = 0;
        int UNI_VAD_SPEECH_END = 1;
        int UNI_VAD_SILENCE_TIMEOUT = 2;
        int UNI_VAD_VOICE_TIMEOUT = 3;
        /* local asr */
        int UNI_LOCAL_ASR_VAD_RESULT = 4;  /*4*/
        int UNI_LOCAL_ASR_SELF_RESULT = 5;
        int UNI_LOCAL_ASR_TIMEOUT = 6;
        int UNI_LOCAL_ASR_FAIL = 7;
        int UNI_LOCAL_ASR_OVERRUN = 8;
        /* remote asr */
        int UNI_REMOTE_ASR_VAD_RESULT = 9; /*9*/
        int UNI_REMOTE_ASR_SELF_RESULT = 10;
        int UNI_REMOTE_ASR_NETWORK_ERROR = 11;
        int UNI_REMOTE_ASR_TIMEOUT = 12;
        int UNI_REMOTE_ASR_FAIL = 13;
        int UNI_REMOTE_ASR_OVERRUN = 14;
        /* audio source capture*/
        int UNI_CAPTURE_FAIL_OPEN_DEVICE = 15;/*15*/
        int UNI_CAPTURE_FAIL_READ = 16;
        int UNI_CAPTURE_CLOSE_DEVICE = 17;
        int UNI_CAPTURE_BUF_OVERRUN = 18;
        int UNI_CAPTURE_TIMEOUT = 19;
        /* TTS */
        int UNI_TTS_FAIL_OPEN_DEVICE = 20;/*20*/
        int UNI_TTS_FAIL_WRITE = 21;
        int UNI_TTS_FAIL_READ_PCM_FILE = 22;
        int UNI_LOCAL_TTS_FAIL = 23;
        int UNI_TTS_NETWORK_ERROR = 24;
        int UNI_TTS_START = 25;
        int UNI_TTS_END = 26;
    }

    public static final String TAG = "newbie";

    public interface OnAsrEventListener {
        void onEvent(int eventId, int time, byte[] eventContent, int eventErrCode, long eventExtendInfo);
    }

    public static OnAsrEventListener mOnAsrEventListener = null;

    public static void setOnAsrEventListener(OnAsrEventListener listener) {
        mOnAsrEventListener = listener;
    }

    /**
     * 识别结果处理
     * eventId:事件id，参考UNI_EVENT_ENUM
     * time:时间（不支持，保留）
     * eventContent:内容
     * eventErrCode:错误妈
     * eventExtendInfo:(不支持，保留)
     */
    public static void AsrEventHandle(int eventId, int time, byte[] eventContent, int eventErrCode, long eventExtendInfo) {
        if (mOnAsrEventListener != null) {
            mOnAsrEventListener.onEvent(eventId, time, eventContent, eventErrCode, eventExtendInfo);
        }
    }

    /**
     * tts 播放处理
     * eventId:事件id，参考UNI_EVENT_ENUM
     * time:时间（不支持，保留）
     * eventContent:内容
     * eventErrCode:错误妈
     * eventExtendInfo:(不支持，保留)
     */
    public static void TtsEventHandle(int eventId, int time, byte[] eventContent, int eventErrCode, long eventExtendInfo) {
        Log.d(TAG, "TtsEventHandle: eventId=" + eventId + ",time=" + time + ",eventContent=" + (new String(eventContent)) + ",eventErrorcode=" + eventErrCode + ",evneyErrInfo=" + eventExtendInfo);
    }


    /**
     * 创建模型编译器
     * asrfixDir:asrfix.dat的目录【声学模型】
     */
    public native static long CreateModeCompiler(byte[] asrfixDir);

    /**
     * 销毁模型编译器
     * modeCompiler:模型编译器
     */
    public native static void DestroyModeCompiler(long modeCompiler);

    /**
     * 加载jsgfClg文件
     * modeCompiler:模型编译器
     * jsgfClgPath:jsgfClg文件路径
     */
    public native static int ModeCompilerLoadJsgfClg(long modeCompiler, byte[] jsgfClgPath);

    /**
     * 编译模型
     * modeCompiler:模型编译器
     * inSlotDatPath:输入槽数据的路径
     * jsgfData:jsgf文本数据
     * vocabData:vocab文本数据
     * outModePath:编译好的模型输出的路径
     * outSlotDatPath:输出本次编译的槽数据的路径，可以与输入槽数据的路径一样
     */
    public native static int CompileMode(long modeCompiler, byte[] inSlotDatPath, byte[] jsgfData, byte[] vocabData, byte[] outModePath, byte[] outSlotDatPath);

    /**
     * 加载模型
     * modePath:模型路径
     */
    public native static int ReLoadMode(byte[] modePath);
}

