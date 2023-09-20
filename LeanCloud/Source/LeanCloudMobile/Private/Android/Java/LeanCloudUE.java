package com.lc;

import cn.leancloud.LCObject;
import cn.leancloud.LCUser;
import cn.leancloud.LeanCloud;
import cn.leancloud.json.JSONObject;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

import android.app.Activity;

import android.os.Handler;
import android.os.Looper;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import java.lang.reflect.Type;
import java.util.Map;

public class LeanCloudUE {
    public static void initialize(final Activity activity, String clientID, String clientToken, String serverURL) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                LeanCloud.initialize(activity, clientID, clientToken, serverURL);
            }
        });
    }

    public static void SetStringForKey(LCObject object, String key, String value) {
        object.put(key, value);
    }

    public static void SetDoubleForKey(LCObject object, String key, double value) {
        object.put(key, value);
    }

    public static void SetIntergerForKey(LCObject object, String key, long value) {
        object.put(key, value);
    }

    public static void SetBooleanForKey(LCObject object, String key, boolean value) {
        object.put(key, value);
    }

    public static String GetClassName(LCObject object) {
        return object.getClassName();
    }

    public static String GetStringFromKey(LCObject object, String key) {
        return object.getString(key);
    }

    public static double GetDoubleFromKey(LCObject object, String key) {
        return object.getDouble(key);
    }

    public static long GetIntergerFromKey(LCObject object, String key) {
        return object.getLong(key);
    }

    public static boolean GetBooleanFromKey(LCObject object, String key) {
        return object.getBoolean(key);
    }

    public static void DeleteValueForKey(LCObject object, String key) {
        object.remove(key);
    }

    public static void Save(LCObject object, int callBackID) {
        // 将对象保存到云端
        object.saveInBackground().subscribe(new Observer<LCObject>() {
            public void onSubscribe(Disposable disposable) {}
            public void onNext(LCObject todo) {
                onLeanCloudBoolResultDelegate(true, "", callBackID);
            }
            public void onError(Throwable throwable) {
                onLeanCloudBoolResultDelegate(false, throwable.getMessage(), callBackID);
            }
            public void onComplete() {}
        });
    }

    public static Map getServerData(LCObject object) {
        return object.getServerData();
    }

    public static LCUser GetCurrentUser() {
        return LCUser.currentUser();
    }

    public static void LogOut() {
        LCUser.logOut();
    }

    public static void LoginAnonymously(int callBackID) {
        LCUser.logInAnonymously().subscribe(new Observer<LCUser>() {
            public void onSubscribe(Disposable disposable) {}
            public void onNext(LCUser user) {
                onLeanCloudUserDelegate(user, "", callBackID);
            }
            public void onError(Throwable throwable) {
                onLeanCloudUserDelegate(null, throwable.getMessage(), callBackID);
            }
            public void onComplete() {}
        });
    }

    public static void LoginWithSessionToken(String SessionToken, int callBackID) {
        LCUser.becomeWithSessionTokenInBackground(SessionToken).subscribe(new Observer<LCUser>() {
            public void onSubscribe(Disposable disposable) {}
            public void onNext(LCUser user) {
                LCUser.changeCurrentUser(user, true);
                onLeanCloudUserDelegate(user, "", callBackID);
            }
            public void onError(Throwable throwable) {
                onLeanCloudUserDelegate(null, throwable.getMessage(), callBackID);
            }
            public void onComplete() {}
        });
    }

    public static void LoginWithAuthData(String AuthDataStr, String Platform, int callBackID) {

        Gson gson = new Gson();
        Type type = new TypeToken<Map<String, Object>>(){}.getType();
        Map<String, Object> thirdPartyData = gson.fromJson(AuthDataStr, type);

        LCUser.loginWithAuthData(thirdPartyData, Platform).subscribe(new Observer<LCUser>() {
            public void onSubscribe(Disposable disposable) {
            }
            public void onNext(LCUser user) {
                onLeanCloudUserDelegate(user, "", callBackID);
            }
            public void onError(Throwable throwable) {
                onLeanCloudUserDelegate(null, throwable.getMessage(), callBackID);
            }
            public void onComplete() {
            }
        });
    }

    public static void AssociateWithAuthData(LCUser User, String AuthDataStr, String Platform, int callBackID) {

        Gson gson = new Gson();
        Type type = new TypeToken<Map<String, Object>>(){}.getType();
        Map<String, Object> thirdPartyData = gson.fromJson(AuthDataStr, type);

        User.associateWithAuthData(thirdPartyData, Platform).subscribe(new Observer<LCUser>() {
            @Override
            public void onSubscribe(Disposable d) {
            }
            @Override
            public void onNext(LCUser user) {
                onLeanCloudBoolResultDelegate(true, "", callBackID);
            }
            @Override
            public void onError(Throwable e) {
                onLeanCloudBoolResultDelegate(false, e.getMessage(), callBackID);
            }
            @Override
            public void onComplete() {
            }
        });
    }

    public static void DisassociateWithPlatform(LCUser User, String Platform, int callBackID) {
        User.dissociateWithAuthData(Platform).subscribe(new Observer<LCUser>() {
            @Override
            public void onSubscribe(Disposable d) {
            }
            @Override
            public void onNext(LCUser user) {
                onLeanCloudBoolResultDelegate(true, "", callBackID);
            }
            @Override
            public void onError(Throwable e) {
                onLeanCloudBoolResultDelegate(false, e.getMessage(), callBackID);
            }
            @Override
            public void onComplete() {
            }
        });
    }

    public static void RetrieveShortToken(int callBackID) {
        LCUser.retrieveShortTokenInBackground(GetCurrentUser().getSessionToken()).subscribe(new Observer<JSONObject>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(JSONObject jsonObject) {
                String credential = jsonObject.getString("identityToken");
                onRetrieveShortTokenSuccess(credential, callBackID);
            }

            @Override
            public void onError(Throwable error) {
                onFail(error.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }


    public native static void onLeanCloudBoolResultDelegate(boolean success, String ErrorReason, int callBackID);
    public native static void onLeanCloudUserDelegate(LCUser user, String ErrorReason, int callBackID);
    public native static void onFail(String ErrorReason, int callBackID);
    public native static void onRetrieveShortTokenSuccess(String content, int callBackID);
}
