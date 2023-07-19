/Users/zgjmlw/Android/sdk/ndk/23.1.7779620/ndk-build  NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk APP_BUILD_SCRIPT=Android.mk
adb forward tcp:23946 tcp:23946
adb push obj/local/armeabi-v7a/hello /data/local/tmp/
adb shell "chmod 777 /data/local/tmp/hello"