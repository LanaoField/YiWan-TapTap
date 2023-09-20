#include "iOS/LCObjcHelper.h"
#include "Tools/LCJsonHelper.h"

FString LCObjcHelper::Convert(NSString *String) {
	return FString(String);
}

NSString * LCObjcHelper::Convert(const FString& String) {
	return String.GetNSString();
}

TArray<FString> LCObjcHelper::Convert(NSArray<NSString *> *strings) {
	TArray<FString> UE_Strings;
	for (NSString *string in strings) {
		UE_Strings.Add(LCObjcHelper::Convert(string));
	}
	return UE_Strings;
}

NSArray<NSString *> * LCObjcHelper::Convert(const TArray<FString>& Strings) {
	NSMutableArray<NSString *> *strings = [NSMutableArray arrayWithCapacity:Strings.Num()];
	for (auto String : Strings) {
		[strings addObject:LCObjcHelper::Convert(String)];
	}
	return strings;
}

TSharedPtr<FJsonObject> LCObjcHelper::Convert(NSDictionary *dic) {
	FString JsonStr = LCObjcHelper::GetJson(dic);
	return LCJsonHelper::GetJsonObject(JsonStr);
}

NSDictionary * LCObjcHelper::Convert(const TSharedPtr<FJsonObject>& JsonObject) {
	if (!JsonObject.IsValid()) {
		return nil;
	}
	FString JsonStr = LCJsonHelper::GetJsonString(JsonObject);
	NSString *jsonString = LCObjcHelper::Convert(JsonStr);
	NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
	NSError *err;
	NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
														options:NSJSONReadingMutableContainers
														  error:&err];
	return dic;
}


FLCError LCObjcHelper::Convert(NSError *Error) {
	FLCError Result;
	Result.Code = Error.code;
	Result.Reason = LCObjcHelper::Convert(Error.localizedDescription);
	return Result;
}

FDateTime LCObjcHelper::Convert(NSDate *date) {
	if (date == nil) {
		return FDateTime();
	}
	double timestamp = [date timeIntervalSince1970];
	return FDateTime::FromUnixTimestamp(timestamp);;
}

FString LCObjcHelper::GetJson(NSDictionary *dic) {
	if ([dic isKindOfClass:[NSDictionary class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return LCObjcHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSDictionary convert error");
	FString tempStr;
	return tempStr;
}

FString LCObjcHelper::GetJson(NSArray *array) {
	if ([array isKindOfClass:[NSArray class]]) {
		NSData *jsonData = [NSJSONSerialization dataWithJSONObject:array options:NSJSONWritingPrettyPrinted error:nil];
		if (jsonData) {
			NSString *dataStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
			return LCObjcHelper::Convert(dataStr);
		}
	}
	NSLog(@"iOSHelper::getJson NSArray convert error");
	FString tempStr;
	return tempStr;
}

TSharedPtr<FLCObject> LCObjcHelper::Convert(LCObject *object) {
	if (![object isKindOfClass:[LCObject class]]) {
		return nullptr;
	}
	TSharedPtr<FLCObject> Object_UE = MakeShared<FLCObject>(object);
	return Object_UE;
}

TSharedPtr<FLCUser> LCObjcHelper::Convert(LCUser *user) {
	if (![user isKindOfClass:[LCObject class]]) {
		return nullptr;
	}
	TSharedPtr<FLCUser> User_UE = MakeShared<FLCUser>(user);
	return User_UE;
}
