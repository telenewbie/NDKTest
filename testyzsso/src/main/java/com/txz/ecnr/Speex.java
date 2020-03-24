package com.txz.ecnr;

public abstract class Speex {
    static{
        System.loadLibrary("txzEcrn");
    }

    /**
     * 创建AEC
     * frame_size:一次处理帧的数量(10ms-20ms)
     * sample_rate:采样率
     * filter_length:回音消除的样本数(100ms-500ms)
     * return:0为失败，非零为AEC的句柄
     */
    public native static long AecCreate(int frame_size, int sample_rate, int filter_length);

    /**
     * 销毁AEC
     * st:AEC的句柄
     */
    public native static void AecDestroy(long st);

    /**
     * AEC处理
     * st:AEC的句柄
     * mic:mic数据
     * ref:重采样数据
     * out:输出数据
     */
    public native static void AecProc(long st, byte[] mic, byte[] ref, byte[] out);

    /**
     * AEC处理，仅输入mic信号(异步处理)
     * st:AEC的句柄
     * mic:mic数据
     * out:输出数据
     */
    public native static void AecProcMic(long st, byte[] mic, byte[] out);

    /**
     * AEC处理，仅输入ref信号(异步处理)
     * st:AEC的句柄
     * ref:重采样数据
     */
    public native static void AecProcRef(long st, byte[] ref);

//********************************preprocess*************************************
    /**创建preprocess[vad,nr,aec]
     * frame_size:一次处理帧的数量(10毫秒-20毫秒)
     * sample_rate:采样率
     * return:0为失败，非0为preprocess的句柄
     * */
    public native static long PreProcCreate(int frame_size, int sample_rate);

    /**销毁PreProc
     * st:PreProc句柄
     * */
    public native static void PreProcDestroy(long st);

    /**PreProc处理数据
     * st:PreProc句柄
     * in:输入数据
     * out:输出数据
     * return：如设置了vad 0为活跃数据，1位静音数据，如没设置vad，无意义
     * */
    public native static int PreProcess(long st, byte[] in, byte[] out);

    /**PreProc虚拟处理数据，用作更新prepro的内部数据，in数据不做处理，比PreProcess减少cpu的消耗。
     * st:PreProc句柄
     * in:输入数据
     * */
    public native static void PreProcessVirtual(long st, byte[] in);

    /**PreProc设置噪声抑制参数
     * st:PreProc句柄
     * enable:0为关闭降噪抑制，1为打开
     * suppress:负数
     * return 0为成功，非0为失败
     * */
    public native static int PreProcSetNr(long st, int enable, int param);

    /**PreProc设置AEC处理器，调用过程 AecProc处理frame_size毫秒后，PreProcess再处理frame_size毫秒，
     * st:PreProc句柄
     * stAec:AEC句柄
     * suppress：AEC抑制参数(负数)
     * return 0为成功，非0为失败
     * */
    public native static int PreProcSetAec(long st, long stAec,int suppress);

    /**PreProc设置VAD
     * st:PreProc句柄
     * enable:0为失能，1为使能
     * silenceToActivityProb:从静音到活跃的[百分比]概率(1~99) ,enable为1生效
     * stayActivityProb:保持活跃的概率(1~99) ,enable为1生效
     * return 0为成功，非0为失败
     * */
    public native static int PreProcSetVad(long st, int enable, int silenceToActivityProb, int stayActivityProb);

}

