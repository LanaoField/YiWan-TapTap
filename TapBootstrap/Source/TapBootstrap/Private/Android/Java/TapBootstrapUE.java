package com.tds;
import android.app.Activity;
import android.os.Handler;
import android.os.Looper;

import com.tapsdk.bootstrap.Callback;
import com.tapsdk.bootstrap.TapBootstrap;
import com.tapsdk.bootstrap.account.TDSUser;
import com.tapsdk.bootstrap.exceptions.TapError;
import com.tapsdk.bootstrap.leaderboard.TDSLeaderBoardRanking;
import com.tds.common.entities.Pair;
import com.tds.common.entities.TapBillboardConfig;
import com.tds.common.entities.TapConfig;
import com.tds.common.entities.TapDBConfig;
import com.tds.common.models.TapRegionType;

import org.jetbrains.annotations.NotNull;

import java.util.List;
import java.util.Map;
import java.util.HashSet;
import java.util.Set;

import cn.leancloud.LCUser;
import cn.leancloud.json.JSONObject;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

public class TapBootstrapUE {
    public static void init(Activity activity, String clientID, String clientToken, String serverUrl, boolean isCN, boolean dbEnable, String channel, String gameVersion, boolean billboardEnable, String billboardUrl, String[] dimensionString) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                TapDBConfig dbConfig = new TapDBConfig();
                dbConfig.setEnable(dbEnable);
                dbConfig.setChannel(channel);
                dbConfig.setGameVersion(gameVersion);

                TapConfig.Builder builder = new TapConfig.Builder().withAppContext(activity)
                        .withServerUrl(serverUrl)
                        .withClientId(clientID)
                        .withClientToken(clientToken)
                        .withRegionType(isCN ? TapRegionType.CN : TapRegionType.IO)
                        .withTapDBConfig(dbConfig);

                if(billboardEnable){
                    Set<Pair<String, String>> dimensionSet = new HashSet<>();
                    for (int i = 0; i < dimensionString.length; i += 2)
                    {
                        dimensionSet.add(Pair.create(dimensionString[i], dimensionString[i+1]));
                    }

                    TapBillboardConfig billboardConfig = new TapBillboardConfig.Builder()
                            .withDimensionSet(dimensionSet)
                            .withServerUrl(billboardUrl)
                            .build();

                    builder.withBillboardConfig(billboardConfig);
                }

                TapConfig tapConfig = builder.build();

                TapBootstrap.init(activity, tapConfig);
            }
        });
    }
    
    public static void queryTapFriendsLeaderBoard(String name, int from, int limit, int callBackID) {
            TDSLeaderBoardRanking.queryTapFriendsLeaderBoard(name, from, limit, new Callback<List<TDSLeaderBoardRanking>>() {
                @Override
                public void onSuccess(List<TDSLeaderBoardRanking> result) {
                    TapBootstrapUE.onRankingsSuccess(result, callBackID);
                }
    
                @Override
                public void onFail(TapError error) {
                    TapBootstrapUE.onRankingsError(error.code, error.detailMessage, callBackID);
                }
            });
        }

    public native static void onRankingsError(int code, String Message, int callBackID);

    public native static void onRankingsSuccess(List<TDSLeaderBoardRanking> result, int callBackID);

}
