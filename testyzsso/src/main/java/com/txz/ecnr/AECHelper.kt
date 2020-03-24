package com.txz.ecnr

import android.content.Context
import com.litesuits.common.io.FileUtils
import com.litesuits.common.io.IOUtils
import com.litesuits.common.utils.SdCardUtil
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.OutputStream
import kotlin.coroutines.CoroutineContext

class AECHelper {
    companion object {
        var aecid: Long? = null
        var nrid: Long? = null
        var file1: File = File(SdCardUtil.getSDCardPath(), "newbieMic.pcm")
        var file2: File = File(SdCardUtil.getSDCardPath(), "newbieRef.pcm")
        var fileAec: File = File(SdCardUtil.getSDCardPath(), "newbieAec.pcm")
        var fileNr: File = File(SdCardUtil.getSDCardPath(), "newbieNr.pcm")
        var fileAecRead: File = File(SdCardUtil.getSDCardPath(), "newbieAecRead.pcm")
        fun create(): Long {
            return Speex.AecCreate(10, 16000, 100).apply {
                aecid = this
                println("aecid:$aecid")
            }
        }

        // 回声消除
        fun cancellation(ctx: Context) {
            // 通过协程进行处理

            //
            //1. 设置参数 aec 和 nr的参数
            openAEC(-100_0000)
            openNR(-1_0000)
            println("file exist:${fileAec.exists()}")
//            println("file delete:${fileAec.de()}")

            if (fileAec.exists()) {
                fileAec.delete()
            }
            if (fileNr.exists()) {
                fileNr.delete()
            }

            // 把文件删掉
            FileUtils.forceDeleteOnExit(file1)
            FileUtils.forceDeleteOnExit(file2)
            FileUtils.forceDeleteOnExit(fileAec)
            aecid?.let {
                // 需要读取
                val buf_once_size = 640
                val buf_out_size = buf_once_size.shr(1) // 右移一位
                var out = ByteArray(buf_out_size) { 0 }
                var mic = ByteArray(buf_out_size) { 0 }
                var ref = ByteArray(buf_out_size) { 0 }
                var nrOut = ByteArray(buf_out_size) { 0 }
                //  640 个字节
                var bufIn = ByteArray(buf_once_size) { 0 }

                val open = ctx.resources.assets.open("testPcm.snd")

                // 双通道 采样位数位16
                // 一次读取 16(采样位数)/8(转换成字节)*2(双通道)* 10(ms)* 16(帧率)= 640 字节
                var readCount = open.read(bufIn)
                var count = 0
                while (readCount > 0) {
                    ++count
//                while (readCount > 0) {
//                    if (++count > 42) break


                    var micCount = 0
                    var refCount = 0
                    bufIn.forEachIndexed { index, byte ->
                        val i = index % 4
                        when (i) {
                            0 -> mic[micCount++] = byte
                            1 -> mic[micCount++] = byte
                            2 -> ref[refCount++] = byte
                            3 -> ref[refCount++] = byte
                        }
                    }
                    Speex.AecProc(it, mic, ref, out)
                    FileUtils.writeByteArrayToFile(fileAec, out, true)

                    Speex.PreProcess(nrid!!, out, nrOut)
                    FileUtils.writeByteArrayToFile(fileNr, nrOut, true)


                    mic = ByteArray(buf_out_size) { 0 }
                    ref = ByteArray(buf_out_size) { 0 }
                    out = ByteArray(buf_out_size) { 0 }
                    bufIn = ByteArray(buf_once_size) { 0 }

                    // nr 处理

                    readCount = open.read(bufIn)
                }
                println("一共遍历：$count")
            }
//            nrProc()
        }

        fun destory() {
            aecid?.run {
                Speex.AecDestroy(this)
            }
        }

        fun procMic() {
//            aecid?.let {
//                Speex.AecProcRef()
//            }
        }

        fun procRef() {
//            aecid?.let {
//                Speex.AecProcRef(it,)
//            }
        }

        fun nrCreate() {
            nrid = Speex.PreProcCreate(10, 16000)
            println("nrid:$nrid")
        }

        fun nrDestory() {
            nrid?.let {
                Speex.PreProcDestroy(it)
            }
        }

        fun nrProc() {
            if (fileNr.exists()) {
                fileNr.delete()
            }
            if (fileAecRead.exists()) {
                fileAecRead.delete()
            }
            FileUtils.forceDeleteOnExit(fileNr)
            nrid?.let { naiad ->
                var outBytes = ByteArray(320) { 0 }
                var bufIn = ByteArray(320) { 0 }
                FileInputStream(fileAec).use { `in` ->
                    println("file in")
                    FileOutputStream(fileNr, true).use { out ->
                        println("file out")
                        var readCount: Int
                        readCount = `in`.read(bufIn)
                        while (readCount > 0) {
                            Speex.PreProcess(naiad, bufIn, outBytes)
                            FileUtils.writeByteArrayToFile(fileAecRead, bufIn, true)
                            out.write(outBytes)

                            bufIn = ByteArray(320) { 0 }
                            outBytes = ByteArray(320) { 0 }

                            readCount = `in`.read(bufIn)
                        }
                        out.flush()

                    }
                }

            }
        }

        fun preProc(`in`: ByteArray, out: ByteArray): Int? {
            return nrid?.let {
                return Speex.PreProcess(it, `in`, out)
            }
        }

        fun openAEC(param: Int) {
            nrid?.let {
                val nrSetAecSuppress = Speex.PreProcSetAec(it, aecid!!, param)
                println("openAEC:$nrSetAecSuppress, param = $param")
            }
        }

        fun openNR(param: Int) {
            nrid?.let {
                val nrSetNoiseSuppress = Speex.PreProcSetNr(it, 1, param)
                println("openNR:$nrSetNoiseSuppress, param = $param")
            }
        }

        fun openVAD(paramSlices: Int, paramStay: Int) {
            nrid?.let {
                val openVAD = Speex.PreProcSetVad(it, 1, paramSlices, paramStay)
                println("openVAD:$openVAD, param = $openVAD")
            }
        }

    }
}