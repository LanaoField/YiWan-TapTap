#pragma once
#include "LCQuery.h"

struct LEANCLOUD_API FLCSaveOption {
	FLCSaveOption() = default;
	FLCSaveOption(const FLCQuery& InQuery, bool bInFetchWhenSave = false);
	FLCSaveOption(const TSharedPtr<FLCQuery>& InQuery, bool bInFetchWhenSave = false);
	void SetFetchWhenSave(bool bInFetchWhenSave);
	void SetMatchQuery(const FLCQuery& InQuery);
	void SetMatchQuery(const TSharedPtr<FLCQuery>& InQuery);
	bool GetFetchWhenSave() const;
	TSharedPtr<FLCQuery> GetMatchQuery() const;
private:
	bool bFetchWhenSave = false;
	TSharedPtr<FLCQuery> MatchQuery = nullptr;
};
