package com.raul.licenta.configuration

import com.raul.licenta.model.UserDetails
import com.raul.licenta.model.UserDetailsData
import org.springframework.security.oauth2.common.DefaultOAuth2AccessToken
import org.springframework.security.oauth2.common.OAuth2AccessToken
import org.springframework.security.oauth2.provider.OAuth2Authentication
import java.util.HashMap

typealias BaseTokenEnhancer = org.springframework.security.oauth2.provider.token.TokenEnhancer

class TokenEnhancer : BaseTokenEnhancer {
    override fun enhance(accessToken: OAuth2AccessToken, authentication: OAuth2Authentication): OAuth2AccessToken {
        val additionalInfo = HashMap<String, Any>()
        additionalInfo[UserDetailsData::class.simpleName!!] = (authentication.principal as UserDetails).data
        (accessToken as DefaultOAuth2AccessToken).additionalInformation = additionalInfo
        return accessToken
    }
}

