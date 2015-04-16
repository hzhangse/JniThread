package com.test;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class JniThreadTestActivity extends Activity {
    /** Called when the activity is first created. */
	private Button mybtn;
	private TextView msgTxt;
	private EditText editTxt;
	private int threadNo=0;
	private long start;
	private static int threadCount=0;
	static{
		System.loadLibrary("JniThread");
	}
	private Handler myHanlder=new Handler(){
		@Override  
		public void handleMessage(Message msg) {  
			 mybtn.setEnabled(true);
			 msgTxt.setVisibility(View.VISIBLE);
	    	 msgTxt.setText("ִ����ɣ������ѣ�"+(System.currentTimeMillis() - start)+"ms");	
		}  

	};
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        editTxt = (EditText)findViewById(R.id.threadNum);
        msgTxt = (TextView)findViewById(R.id.msgTxt);
        mybtn = (Button)findViewById(R.id.button);
        mybtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
               //����JNI�еĺ���������JNI�е��߳�
            	msgTxt.setVisibility(View.VISIBLE);
            	msgTxt.setText("��ȴ�...");
            	threadCount=0;
            	String thread_num =editTxt.getText().toString();
            	if(thread_num == null || "".equals(thread_num)){
            		Toast.makeText(JniThreadTestActivity.this, "�������߳���", Toast.LENGTH_LONG);
            		return;
            	}
            	try{
            		threadNo =Integer.parseInt(thread_num);
            	}catch(Exception ex){
            		Toast.makeText(JniThreadTestActivity.this, "�߳����Ƿ�������������", Toast.LENGTH_LONG);
            		System.out.println(ex.toString());
            		return;
            	}
            	mybtn.setEnabled(false);
            	start = System.currentTimeMillis();
                mainThread(threadNo);
                while(true){
                	if(threadCount ==threadNo){
                		myHanlder.sendEmptyMessage(0);
                		break;
                	}
                }
              }
            });
        //��ʼ��JNI����
        setJNIEnv();
    }
    //��JNI�е��̻߳ص�
    private synchronized void fromJNI(int i)
    {
     Log.v("Java------>", ""+i);
     System.out.println("Java------>"+i);
     threadCount++;
    }
    
    public void showMsg(){
//        if(myHanlder != null){
//       	 myHanlder.sendEmptyMessage(0);
//        }
    }
    private native void mainThread(int num);
    private native void setJNIEnv();
}