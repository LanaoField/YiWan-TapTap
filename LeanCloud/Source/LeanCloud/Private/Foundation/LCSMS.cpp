#include "LCSMS.h"

#include "Network/LCAppRouter.h"
#include "Network/LCHttpClient.h"
#include "Network/LCHttpRequest.h"
#include "Network/LCHttpResponse.h"
#include "Tools/LCHelper.h"

TLCMap FLCSMSOption::GenerateParas() const {
	TLCMap Paras = TemplateVariables;
	if (TTL > 0) {
		Paras.Add("ttl", TTL);
	}
	switch (Type) {
	case ELCSMSType::Text:
		Paras.Add("smsType", "sms");
		break;
	case ELCSMSType::Voice:
		Paras.Add("smsType", "voice");
		break;
	}
	if (!ValidationToken.IsEmpty()) {
		Paras.Add("validate_token", ValidationToken);
	}
	if (!TemplateName.IsEmpty()) {
		Paras.Add("template", TemplateName);
	}
	if (!SignatureName.IsEmpty()) {
		Paras.Add("sign", SignatureName);
	}
	if (!ApplicationName.IsEmpty()) {
		Paras.Add("name", ApplicationName);
	}
	if (!Operation.IsEmpty()) {
		Paras.Add("op", Operation);
	}
	return MoveTemp(Paras);
}

void FLCSMS::RequestSMSCode(const FString& PhoneNumber, const FLCSMSOption& Option,
                            const FLeanCloudBoolResultDelegate& CallBack) {
	TSharedPtr<FLCApplication> ApplicationPtr = FLCApplication::DefaultPtr;
	if (Option.ApplicationPtr.IsValid()) {
		ApplicationPtr = Option.ApplicationPtr;
	}
	if (!ApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(ApplicationPtr->AppRouter->GetRouteUrl("requestSmsCode"));
	TLCMap Paras = Option.GenerateParas();
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	Request.BodyParameters = Paras;
	ApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                    [=](const FLCHttpResponse& InResponse) {
			                                    InResponse.PerformBoolDelegateByResponse(CallBack);
		                                    }));
}

void FLCSMS::VerifySMSCode(const FString& Code, const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr) {
	TSharedPtr<FLCApplication> ApplicationPtr = FLCApplication::DefaultPtr;
	if (AppPtr.IsValid()) {
		ApplicationPtr = AppPtr;
	}
	if (!ApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(ApplicationPtr->AppRouter->GetRouteUrl(FString("verifySmsCode") / Code));
	Request.BodyParameters.Add("mobilePhoneNumber", PhoneNumber);
	ApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
											[=](const FLCHttpResponse& InResponse) {
												InResponse.PerformBoolDelegateByResponse(CallBack);
											}));
}
