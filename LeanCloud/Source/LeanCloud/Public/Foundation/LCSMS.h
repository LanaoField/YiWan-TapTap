#pragma once
#include "LCObject.h"
// #include "LCSMSOption.h"

enum class ELCSMSType {
	Text,
	Voice,
};

struct LEANCLOUD_API FLCSMSOption {
	/**
	 Time to live of validation information, in minutes. Defaults to 10 minutes.
	 */
	int64 TTL = 0;

	/**
	The representation or form of short message.
	*/
	ELCSMSType Type = ELCSMSType::Text;

	/**
	Token used to validate short message request.
	*/
	FString ValidationToken;

	/**
	Template name of text short message.

	@note If not specified, the default validation message will be requested.
	*/
	FString TemplateName;

	/**
	A set of key value pairs that will fill in template placeholders.

	@note You should not use the placeholders listed here in your template:
	`mobilePhoneNumber`, `ttl`, `smsType`, `template` and `sign`.
	*/
	TLCMap TemplateVariables;

	/**
	Signature name of text short message.

	It will be placed ahead of text short message.
	*/
	FString SignatureName;

	/**
	Application name showed in validation message.

	It fills the placeholder <code>{{name}}</code> in default validation message template.
	If not given, the application name in LeanCloud console will be used.
	*/
	FString ApplicationName;

	/**
	The operation description showed in validation message.

	It fills the placeholder <code>{{op}}</code> in default validation message template.
	*/
	FString Operation;

	/**
	Which Application
	*/
	TSharedPtr<FLCApplication> ApplicationPtr;

	TLCMap GenerateParas() const;
};


class LEANCLOUD_API FLCSMS {
public:
	static void RequestSMSCode(const FString& PhoneNumber, const FLCSMSOption& Option, const FLeanCloudBoolResultDelegate& CallBack);
	static void VerifySMSCode(const FString& Code, const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
};
