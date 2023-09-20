#pragma once
#include "Dom/JsonObject.h"

// @see https://leancloud.cn/docs/error_code.html
enum class ELCErrorCode {
	OK = 0,
	// ServerErrorCode
	ObjectNotFound = 101,
	SessionConflict = 4111,
	SessionTokenExpired = 4112,
	// session/client
	CommandTimeout = 9000,
	ConnectionLost = 9001,
	ClientNotOpen = 9002,
	CommandInvalid = 9003,
	CommandDataLengthTooLong = 9008,
	// conversation
	ConversationNotFound = 9100,
	UpdatingMessageNotAllowed = 9120,
	UpdatingMessageNotSent = 9121,
	OwnerPromotionNotAllowed = 9130,
	// other
	NotFound = 9973,
	InvalidType = 9974,
	MalformedData = 9975,
	Inconsistency = 9976,
	UnderlyingError = 9977,

	NoApplication = 10000,
	NoObject = 10001,
	CircularReference = 10002,
	RequestNetworkError = 10003,
	RequestClientError = 10004,
	ParasInvaild = 10005,

};

inline const TCHAR* LexToString(ELCErrorCode ErrorCode) {
	switch (ErrorCode) {
	case ELCErrorCode::ObjectNotFound: return TEXT("Object not found"); break;
	case ELCErrorCode::CommandTimeout: return TEXT("Out command timeout"); break;
	case ELCErrorCode::ConnectionLost: return TEXT("Connection lost"); break;
	case ELCErrorCode::ClientNotOpen: return TEXT("IM client not open"); break;
	case ELCErrorCode::CommandInvalid: return TEXT("In command invalid"); break;
	case ELCErrorCode::CommandDataLengthTooLong: return TEXT("Data length of out command is too long"); break;
	case ELCErrorCode::ConversationNotFound: return TEXT("Conversation not found"); break;
	case ELCErrorCode::UpdatingMessageNotAllowed: return TEXT("Updating message from others is not allowed"); break;
	case ELCErrorCode::UpdatingMessageNotSent: return TEXT("Message is not sent"); break;
	case ELCErrorCode::OwnerPromotionNotAllowed: return TEXT("Updating a member's role to owner is not allowed"); break;
	case ELCErrorCode::NotFound: return TEXT("Not found"); break;
	case ELCErrorCode::InvalidType: return TEXT("Data type invalid"); break;
	case ELCErrorCode::MalformedData: return TEXT("Data format invalid"); break;
	case ELCErrorCode::Inconsistency: return TEXT("Internal inconsistency exception"); break;
	case ELCErrorCode::NoApplication: return TEXT("Application not specified"); break;
	case ELCErrorCode::NoObject: return TEXT("Object is Invalid"); break;
	case ELCErrorCode::CircularReference: return TEXT("Circular reference."); break;
	case ELCErrorCode::RequestNetworkError: return TEXT("Request Network Error."); break;
	case ELCErrorCode::RequestClientError: return TEXT("Request Client Error."); break;
	case ELCErrorCode::ParasInvaild: return TEXT("Parameters Invaild."); break;

		default: return TEXT("");
	}
}

struct LEANCLOUD_API FLCError {
	DECLARE_DELEGATE_OneParam(FDelegate, const FLCError& Error);
	FLCError() = default;
	FLCError(int InCode, const FString& InReason) : Code(InCode), Reason(InReason) {};
	FLCError(ELCErrorCode InCode) {
		Code = (int)InCode;
		Reason = LexToString(InCode);
	};
	FORCEINLINE static void Throw(const FString& InMsg) {
		throw FLCError(-1, InMsg);
	}
	FORCEINLINE static void Throw(int InCode, const FString& InMsg) {
		throw FLCError(InCode, InMsg);
	}
	FORCEINLINE static void Throw(ELCErrorCode InCode) {
		throw FLCError(InCode);
	}
	int Code = 0;
	FString Reason;
	TSharedPtr<FJsonObject> UserInfo;
};
