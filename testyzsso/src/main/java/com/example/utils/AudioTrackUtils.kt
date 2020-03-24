package com.example.utils

import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack
import android.os.Build
import android.os.SystemClock
import java.util.concurrent.atomic.AtomicBoolean

class AudioTrackUtils {

    private var isWriting = AtomicBoolean(false)
    private var isRelease = AtomicBoolean(false)
    private var mMinBuffSize: Int? = null
    private var mAudioTrack: AudioTrack? = null

    class Builder {
        //int sampleRateInHz, int channelConfig, int audioFormat
        private var mSampleRateInHz: Int = 0
        private var mChannelConfig: Int = 0
        private var mAudioFormat: Int = 0

        //音频流类型
        private val mStreamType = AudioManager.STREAM_MUSIC
        //STREAM的意思是由用户在应用程序通过write方式把数据一次一次得写到audiotrack中。这个和我们在socket中发送数据一样，
        // 应用层从某个地方获取数据，例如通过编解码得到PCM数据，然后write到audiotrack。
        private val mMode = AudioTrack.MODE_STREAM
        private var mBufSize = 0

        fun setChannelConfig(channelConfig: Int): Builder {
            mChannelConfig = when (channelConfig) {
                1 -> AudioFormat.CHANNEL_OUT_MONO
                else -> AudioFormat.CHANNEL_OUT_STEREO
            }
            return this
        }

        fun setAudioFormat(audioFormat: Int): Builder {
            mAudioFormat = when (audioFormat) {
                16 -> AudioFormat.ENCODING_PCM_16BIT
                8 -> AudioFormat.ENCODING_PCM_8BIT
                else -> AudioFormat.ENCODING_PCM_16BIT
            }
            return this
        }

        fun setSampleRateInHz(sampleRateInHz: Int): Builder {
            mSampleRateInHz = sampleRateInHz
            return this
        }

        fun setBufSize(size: Int): Builder {
            mBufSize = size
            return this
        }

        fun build(): AudioTrackUtils {
            audioTrack.mMinBuffSize = when (this.mBufSize) {
                0 -> AudioTrack.getMinBufferSize(mSampleRateInHz, mChannelConfig, mAudioFormat)
                else -> mBufSize
            }
            println(audioTrack.mMinBuffSize!!)
            //AudioManager.STREAM_MUSIC
            //STREAM的意思是由用户在应用程序通过write方式把数据一次一次得写到audiotrack中。这个和我们在socket中发送数据一样，
            // 应用层从某个地方获取数据，例如通过编解码得到PCM数据，然后write到audiotrack。
            audioTrack.mAudioTrack = AudioTrack(mStreamType, mSampleRateInHz, mChannelConfig, mAudioFormat, audioTrack.mMinBuffSize!!, mMode)
            return audioTrack
        }
    }

    companion object {
        val audioTrack = AudioTrackUtils()
    }

    fun startPlaying() {
        mAudioTrack?.play()
    }

    fun stopPlaying() {
        mAudioTrack?.stop()
    }

    fun pausePlaying() {
        mAudioTrack?.pause()
    }

    /**
     * AudioTrack#STATE_INITIALIZED  　　//已经初始化
    AudioTrack#STATE_UNINITIALIZED  //没有初始化
     */
    fun getState(): Int? {
        return mAudioTrack?.state
    }

    /**
     * AudioTrack#RECORDSTATE_STOPPED    //停止录制
    AudioTrack#RECORDSTATE_RECORDING    //正在录制
     */
    fun getPlayingState(): Int? {
        return mAudioTrack?.playState
    }


    //读取数据
    fun writeData(byte: ByteArray, startIndex: Int, length: Int): Int {
        // 判断是否写入完毕
        var readSize = 0
        while (readSize < (length - startIndex)) {
            isWriting.set(true)
            if (isRelease.get())
                break
            var readCount = mAudioTrack?.write(byte, startIndex, length)!!
            if (readCount == AudioTrack.ERROR_INVALID_OPERATION || readCount == AudioTrack.ERROR_BAD_VALUE) {
                continue
            }
            if (readCount != 0 && readCount != -1) {//一边播放一边写入语音数据
                //判断AudioTrack未初始化，停止播放的时候释放了，状态就为STATE_UNINITIALIZED
                if (getState() == AudioTrack.STATE_UNINITIALIZED) {
                    // 初始化
                    break
                }
                readSize += readCount
            }
            println("$readSize/$length")
            isWriting.set(false)
        }
        return readSize
    }

    fun closeAudioTrack() {

        while (isWriting.get()) {
            //需要等待写完，才能继续操作
            SystemClock.sleep(10)
            continue
        }
        isRelease.set(true)
        mAudioTrack?.release()
        isWriting.set(false)
        mAudioTrack = null
    }

}