#pragma once
#include "LCError.h"
#include "LCLeaderboardModel.h"


class FLCHttpResponse;

class LEANCLOUD_API FLCLeaderboard {
public:
	enum Type {
		User,
		Entity,
		Object,
	};

	DECLARE_DELEGATE_OneParam(FStatisticsDelegate, const TArray<FLCLeaderboardStatistic>& Statistics);
	DECLARE_DELEGATE_TwoParams(FRankingsDelegate, const TArray<FLCLeaderboardRanking>& Rankings, int64 Count);

	/// The start positon of the query, default is `0`.
	int64 Skip = 0;
	/// The max results count of the query, default is `20`.
	int64 Limit = 20;
	/// Whether to return the count of this leaderboard, default is `false`.
	bool WithCount = false;
	/// The version of the leaderboard, default is `-1`.
	int64 Version = -1;
	/// The statistics of the other leaderboards will be returned, default is Empty.
	TArray<FString> IncludeStatistics;
	/// select member(user or object) keys(optional), default is Empty.
	TArray<FString> SelectMemberKeys;

	void SetApplicationPtr(const TSharedPtr<FLCApplication>& InPtr);
	TSharedPtr<FLCApplication> GetApplicationPtr() const;

	FLCLeaderboard(const FString& Name, const Type& InMemberType = User);

	/**
	 * @brief update user's statistic
	 * @param Values statistics
	 */
	static void UpdateCurrentUserStatistics(const TMap<FString, double> Values,
	                                        const FStatisticsDelegate& OnSuccess, const FLCError::FDelegate& OnError,
	                                        const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	/**
	 * @brief update user's statistic
	 * @param Values statistics
	 */
	static void UpdateStatistics(const TSharedPtr<FLCUser>& User, const TMap<FString, double> Values,
	                             const FStatisticsDelegate& OnSuccess, const FLCError::FDelegate& OnError,
	                             const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	/**
	 * @brief get member statistics
	 * @param TargetID member objectId
	 * @param StatisticNames statistic names
	 * @param InMemberType @see FLCLeaderboard::Type
	 */
	static void GetStatistics(const FString& TargetID, const FStatisticsDelegate& OnSuccess,
	                          const FLCError::FDelegate& OnError, TArray<FString> StatisticNames = {},
	                          const Type& InMemberType = User, const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	/**
	 * @brief get leaderboard results.
	 */
	void GetResults(const FRankingsDelegate& OnSuccess, const FLCError::FDelegate& OnError);

	/**
	 * @brief get leaderboard results around target id(user, object or entity).
	 * @param TargetID member objectId
	 */
	void GetAroundResults(const FString& TargetID, const FRankingsDelegate& OnSuccess,
	                      const FLCError::FDelegate& OnError);

	FString GetStatisticName() const;
	Type GetMemberType() const;

protected:
	FString StatisticName;
	Type MemberType;
	TWeakPtr<FLCApplication> ApplicationPtr;

private:
	FLCLeaderboard() = default;

	static void HandleStatisticsCallback(const FStatisticsDelegate& OnSuccess, const FLCError::FDelegate& OnError,
	                                     const FLCHttpResponse& InResponse, const TSharedPtr<FLCApplication>& AppPtr);
};
