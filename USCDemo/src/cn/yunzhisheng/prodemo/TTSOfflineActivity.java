package cn.yunzhisheng.prodemo;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import cn.yunzhisheng.prodemo.sdk.Utils;
import com.unisound.client.SpeechConstants;
import com.unisound.client.SpeechSynthesizer;
import com.unisound.client.SpeechSynthesizerListener;
import java.io.File;

public class TTSOfflineActivity extends Activity {
	private static boolean TTS_PLAY_FLAGE = false;

	private EditText mTTSText;
	private Button mTTSPlayBtn;
	private SpeechSynthesizer mSynthesizer;

	private String mFrontendModel;
	private String mBackendModel;

	private String MODEL_PATH_NAME;

	/*assets中前端声学模型文件名*/
	private final String FRONTEND_MODEL = "frontend_model_offline_v9.0.0";
	/*assets中后端声学模型文件名*/
	private final String BACKEND_MODEL = "backend_kiyo_lpc2wav_22k_pf_mixed_v1.0.0";
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
		super.onCreate(savedInstanceState);
		initView();
		// 初始化本地TTS播报
		initTts();
	}

	private void initView() {
		setContentView(R.layout.activity_offline_tts);
		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.status_bar_main);

		mTTSText = findViewById(R.id.textViewResult);
		mTTSPlayBtn = findViewById(R.id.recognizer_btn);
		mTTSPlayBtn.setEnabled(false);
		mTTSPlayBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				TTSPlay();
			}
		});
	}

	/**
	 * 初始化本地离线TTS
	 */
	private void initTts() {
		// 初始化语音合成对象
		mSynthesizer = new SpeechSynthesizer(this, Config.appKey, Config.secret);
		setModel();

		// 设置本地合成
		mSynthesizer.setOption(SpeechConstants.TTS_SERVICE_MODE, SpeechConstants.TTS_SERVICE_MODE_LOCAL);
		mSynthesizer.setOption(SpeechConstants.TTS_KEY_SAMPLE_RATE, 22050);
		// 设置回调监听
		mSynthesizer.setTTSListener(mListener);
		// 初始化合成引擎
		mSynthesizer.init("");
	}

	private SpeechSynthesizerListener mListener = new SpeechSynthesizerListener() {
		@Override
		public void onEvent(int type) {
			switch (type) {
				case SpeechConstants.TTS_EVENT_INIT:
					// 初始化成功回调
					mTTSPlayBtn.setEnabled(true);
					break;
				case SpeechConstants.TTS_EVENT_SYNTHESIZER_START:
					// 开始合成回调
					break;
				case SpeechConstants.TTS_EVENT_SYNTHESIZER_END:
					// 合成结束回调
					break;
				case SpeechConstants.TTS_EVENT_BUFFER_BEGIN:
					// 开始缓存回调
					break;
				case SpeechConstants.TTS_EVENT_BUFFER_READY:
					// 缓存完毕回调
					break;
				case SpeechConstants.TTS_EVENT_PLAYING_START:
					// 开始播放回调
					break;
				case SpeechConstants.TTS_EVENT_PLAYING_END:
					// 播放完成回调
					setTTSButtonReady();
					break;
				case SpeechConstants.TTS_EVENT_PAUSE:
					// 暂停回调
					break;
				case SpeechConstants.TTS_EVENT_RESUME:
					// 恢复回调
					break;
				case SpeechConstants.TTS_EVENT_STOP:
					// 停止回调
					break;
				case SpeechConstants.TTS_EVENT_RELEASE:
					// 释放资源回调
					break;
				default:
					break;
			}

		}

		@Override
		public void onError(int type, String errorMSG) {
			// 语音合成错误回调
			toastMessage(errorMSG);
			setTTSButtonReady();
		}
	};

	private void setModel() {
		MODEL_PATH_NAME = new File(getFilesDir(), "YunZhiSheng/tts/").getAbsolutePath();
		//从assets加载前后端模型
		Utils.copyFile(this, FRONTEND_MODEL, MODEL_PATH_NAME);
		Utils.copyFile(this, BACKEND_MODEL, MODEL_PATH_NAME);

		mFrontendModel = MODEL_PATH_NAME + "/"+ FRONTEND_MODEL;
		mBackendModel = MODEL_PATH_NAME + "/" + BACKEND_MODEL;
		// 设置前端模型
		mSynthesizer.setOption(SpeechConstants.TTS_KEY_FRONTEND_MODEL_PATH, mFrontendModel);
		// 设置后端模型
		mSynthesizer.setOption(SpeechConstants.TTS_KEY_BACKEND_MODEL_PATH, mBackendModel);
	}

	private void TTSPlay() {
		if (!TTS_PLAY_FLAGE) {
			mSynthesizer.playText(mTTSText.getText().toString());
			setTTSButtonStop();
		} else {
			mSynthesizer.stop();
			setTTSButtonReady();
		}
	}

	private void setTTSButtonStop() {
		TTS_PLAY_FLAGE = true;
		mTTSPlayBtn.setText(R.string.stop_tts);
	}

	private void setTTSButtonReady() {
		mTTSPlayBtn.setText(R.string.start_tts);
		TTS_PLAY_FLAGE = false;
	}

	@Override
	public void onPause() {
		super.onPause();
		// 主动停止识别
		if (mSynthesizer != null) {
			mSynthesizer.stop();
		}
	}

	@Override
	protected void onDestroy() {
		// 主动释放离线引擎
		if (mSynthesizer != null) {
			mSynthesizer.release(SpeechConstants.TTS_RELEASE_ENGINE, null);
		}
		super.onDestroy();
	}

	private void toastMessage(String message) {
		Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
	}
}
