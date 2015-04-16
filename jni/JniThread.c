#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#include<jni.h>
#include<android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

//�߳���
#define NUMTHREADS 5
//ȫ�ֱ���
JavaVM *g_jvm = NULL;
jobject g_obj = NULL;
void *thread_fun(void* arg) {
	JNIEnv *env;
	jclass cls;
	jmethodID mid;

	//Attach���߳�
	if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {
		LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
		return NULL ;
	}
	//�ҵ���Ӧ����
	cls = (*env)->GetObjectClass(env, g_obj);
	if (cls == NULL) {
		LOGE("FindClass() Error.....");
		goto error;
	}
	//�ٻ�����еķ���
	mid = (*env)->GetMethodID(env, cls, "fromJNI", "(I)V");
	if (mid == NULL) {
		LOGE("GetMethodID() Error.....");
		goto error;
	}
	//������java�еľ�̬����
	(*env)->CallVoidMethod(env, cls, mid, (int) arg);

	error:
	//Detach���߳�
	if ((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
		LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
	}

	pthread_exit(0);
}

//��java�����Դ������߳�
JNIEXPORT void Java_com_test_JniThreadTestActivity_mainThread(JNIEnv* env,
		jobject obj, jint threadNum) {
	int i;
	pthread_t* pt;
	pt = (pthread_t*) malloc(threadNum * sizeof(pthread_t));
	for (i = 0; i < threadNum; i++) {
		//�������߳�
		pthread_create(&pt[i], NULL, &thread_fun, (void *) i);
	}

	for (i = 0; i < threadNum; i++) {
		pthread_join(pt[i], NULL);
	}

	LOGE("main thread exit.....");
}

//��java����������JNI����
JNIEXPORT void Java_com_test_JniThreadTestActivity_setJNIEnv(JNIEnv* env,
		jobject obj) {
	//����ȫ��JVM�Ա������߳���ʹ��
	(*env)->GetJavaVM(env, &g_jvm);
	//����ֱ�Ӹ�ֵ(g_obj = obj)
	g_obj = (*env)->NewGlobalRef(env, obj);
}

//����̬�ⱻ����ʱ���������ϵͳ����
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv* env = NULL;
	jint result = -1;

	//��ȡJNI�汾
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("GetEnv failed!");
		return result;
	}

	return JNI_VERSION_1_4;
}
