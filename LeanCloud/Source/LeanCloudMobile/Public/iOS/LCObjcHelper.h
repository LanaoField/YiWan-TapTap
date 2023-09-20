#pragma once
#import <Foundation/Foundation.h>
#include "LCError.h"
#include "LCUser.h"

class LEANCLOUDMOBILE_API LCObjcHelper {
public:
	static FString Convert(NSString *String);
	static NSString * Convert(const FString& String);

	static TArray<FString> Convert(NSArray<NSString *> *strings);
	static NSArray<NSString *> * Convert(const TArray<FString>& Strings);

	static TSharedPtr<FJsonObject> Convert(NSDictionary *dic);

	static NSDictionary * Convert(const TSharedPtr<FJsonObject>& JsonObject);
	
	static FLCError Convert(NSError *Error);

	static FDateTime Convert(NSDate *date);

	static FString GetJson(NSDictionary *dic);
    
	static FString GetJson(NSArray *array);

	static TSharedPtr<FLCObject> Convert(LCObject *object);

	static TSharedPtr<FLCUser> Convert(LCUser *user);
};
