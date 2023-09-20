#include "LCSaveOption.h"

FLCSaveOption::FLCSaveOption(const FLCQuery& InQuery, bool bInFetchWhenSave) : bFetchWhenSave(bInFetchWhenSave) {
	MatchQuery = MakeShared<FLCQuery>(InQuery);
}

FLCSaveOption::FLCSaveOption(const TSharedPtr<FLCQuery>& InQuery, bool bInFetchWhenSave)  : bFetchWhenSave(bInFetchWhenSave), MatchQuery(InQuery){}

void FLCSaveOption::SetMatchQuery(const TSharedPtr<FLCQuery>& InQuery) {
	MatchQuery = InQuery;
}

void FLCSaveOption::SetFetchWhenSave(bool bInFetchWhenSave) {
	bFetchWhenSave = bInFetchWhenSave;
}

void FLCSaveOption::SetMatchQuery(const FLCQuery& InQuery) {
	MatchQuery = MakeShared<FLCQuery>(InQuery);
}

bool FLCSaveOption::GetFetchWhenSave() const {
	return bFetchWhenSave;
}

TSharedPtr<FLCQuery> FLCSaveOption::GetMatchQuery() const {
	return MatchQuery;
}
