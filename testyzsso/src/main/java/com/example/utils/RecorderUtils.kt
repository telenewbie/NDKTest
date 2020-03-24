package com.example.utils

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder

import java.lang.annotation.Retention

class RecorderUtils private constructor() {

    private var mRecordBufferSize: Int? = null
    private var mAudioRecord: AudioRecord? = null


    class Builder {
        //int sampleRateInHz, int channelConfig, int audioFormat
        private var mSampleRateInHz: Int = 0
        private var mChannelConfig: Int = 0
        private var mAudioFormat: Int = 0

        fun setChannelConfig(channelConfig: Int): Builder {
            mChannelConfig = channelConfig
            return this
        }

        fun setAudioFormat(audioFormat: Int): Builder {
            mAudioFormat = audioFormat
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


}
