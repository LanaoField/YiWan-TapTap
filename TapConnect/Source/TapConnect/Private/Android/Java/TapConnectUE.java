package com.tds;

import android.app.Activity;

import android.os.Handler;
import android.os.Looper;

import com.tapsdk.tapconnect.TapConnect;

public class TapConnectUE {

  public static void init(Activity activity, String clientId, String clientToken, boolean isCN) {
           Handler mainHandler = new Handler(Looper.getMainLooper());
          mainHandler.post(new Runnable() {
              @Override
              public void run() {
                  TapConnect.init(activity, clientId, clientToken, isCN);
              }
          });
  }

  public static void setEntryVisible(boolean enable) {
            Handler mainHandler = new Handler(Looper.getMainLooper());
            mainHandler.post(new Runnable() {
                @Override
                public void run() {
                    TapConnect.setEntryVisible(enable);
                }
            });
  }
}
