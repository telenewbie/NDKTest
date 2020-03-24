package com.example.utils

import android.media.AudioFormat.*
import android.media.AudioRecord
import android.media.MediaRecorder

class RecorderUtils private constructor() {

    private var mRecordBufferSize: Int? = null
    private var mAudioRecord: AudioRecord? = null

    private var mWhetherRecord = false


    class Builder {
        //int sampleRateInHz, int channelConfig, int audioFormat
        private var mSampleRateInHz: Int = 0
        private var mChannelConfig: Int = 0
        private var mAudioFormat: Int = 0

        fun setChannelConfig(channelConfig: Int): Builder {
            mChannelConfig = when (channelConfig) {
                1 -> CHANNEL_IN_MONO
                else -> CHANNEL_IN_STEREO
            }
            return this
        }

        fun setAudioFormat(audioFormat: Int): Builder {
            mAudioFormat = when (audioFormat) {
                16 -> ENCODING_PCM_16BIT
                8 -> ENCODING_PCM_8BIT
                else -> ENCODING_PCM_16BIT
            }
            return this
        }

        fun setSampleRateInHz(sampleRateInHz: Int): Builder {
            mSampleRateInHz = sampleRateInHz
            return this
        }

        fun build(): RecorderUtils {
            recorder.mRecordBufferSize = AudioRecord.getMinBufferSize(mSampleRateInHz, mChannelConfig, mAudioFormat)
            recorder.mAudioRecord = AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRateInHz, mChannelConfig, mAudioFormat, recorder.mRecordBufferSize!!)
            return recorder
        }
    }

    companion object {
        val recorder = RecorderUtils()
    }

    fun startRecording() {
        mAudioRecord?.startRecording()
        openTagRecording()
    }

    fun stopRecording() {
        closeTagRecording()
        mAudioRecord?.stop()
    }

    private fun openTagRecording() {
        mWhetherRecord = true
    }

    private fun closeTagRecording() {
        mWhetherRecord = false
    }

    fun isRecording(): Boolean {
        return mWhetherRecord
    }

    /**
     * AudioRecord#STATE_INITIALIZED  　　//已经初始化
    AudioRecord#STATE_UNINITIALIZED  //没有初始化
     */
    fun getState(): Int? {
        return mAudioRecord?.state
    }

    /**
     * AudioRecord#RECORDSTATE_STOPPED    //停止录制
    AudioRecord#RECORDSTATE_RECORDING    //正在录制
     */
    fun getRecordingState(): Int? {
        return mAudioRecord?.recordingState
    }

    //读取数据
    fun readData(byte: ByteArray, startIndex: Int, length: Int): Int {
        return mAudioRecord?.read(byte, startIndex, length) ?: -1//读取流
    }

    fun closeRecorder() {
        mAudioRecord?.release()
        mAudioRecord = null
    }


}
