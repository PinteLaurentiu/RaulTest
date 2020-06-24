package com.raul.licenta.configuration

import com.fasterxml.jackson.databind.ObjectMapper
import com.raul.licenta.model.UserDetails
import com.raul.licenta.model.UserDetailsData
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken
import org.springframework.security.core.Authentication
import org.springframework.security.oauth2.common.exceptions.InvalidTokenException
import org.springframework.security.oauth2.provider.OAuth2Authentication
import org.springframework.security.oauth2.provider.token.store.JwtAccessTokenConverter

class AccessTokenConverter : JwtAccessTokenConverter() {
    override fun extractAuthentication(claims: Map<String, *>): OAuth2Authentication? {
        val oAuth2Authentication = super.extractAuthentication(claims)
        val authentication: Authentication = oAuth2Authentication.userAuthentication ?: return oAuth2Authentication
        val userDetailsMap = claims[UserDetailsData::class.simpleName] as? Map<*, *> ?:
            throw InvalidTokenException("JWT has invalid format!")
        val objectMapper = ObjectMapper()
        return OAuth2Authentication(oAuth2Authentication.oAuth2Request, UsernamePasswordAuthenticationToken(
                UserDetails(
                        objectMapper.readValue(
                                objectMapper.writeValueAsString(userDetailsMap),
                                UserDetailsData::class.java),
                        null),
                authentication.credentials,
                authentication.authorities))
    }
}
