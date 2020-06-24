package com.raul.licenta.configuration

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.context.annotation.Bean
import org.springframework.context.annotation.Configuration
import org.springframework.context.annotation.Primary
import org.springframework.security.authentication.AuthenticationManager
import org.springframework.security.crypto.password.PasswordEncoder
import org.springframework.security.oauth2.config.annotation.configurers.ClientDetailsServiceConfigurer
import org.springframework.security.oauth2.config.annotation.web.configuration.AuthorizationServerConfigurerAdapter
import org.springframework.security.oauth2.config.annotation.web.configuration.EnableAuthorizationServer
import org.springframework.security.oauth2.config.annotation.web.configurers.AuthorizationServerEndpointsConfigurer
import org.springframework.security.oauth2.config.annotation.web.configurers.AuthorizationServerSecurityConfigurer
import org.springframework.security.oauth2.provider.token.DefaultTokenServices
import org.springframework.security.oauth2.provider.token.TokenEnhancerChain
import org.springframework.security.oauth2.provider.token.TokenStore
import org.springframework.security.oauth2.provider.token.store.JwtAccessTokenConverter
import org.springframework.security.oauth2.provider.token.store.JwtTokenStore

@Configuration
@EnableAuthorizationServer
class AuthorizationServerConfig(
        @Autowired
        val authenticationManager: AuthenticationManager,

        @Autowired
        val oauthClientPasswordEncoder: PasswordEncoder
) : AuthorizationServerConfigurerAdapter() {
    @Bean
    fun accessTokenConverter(): JwtAccessTokenConverter {
        val converter = AccessTokenConverter()
        converter.setSigningKey("RBsknQZt2U0Td5tY7mBl")
        return converter
    }

    @Bean
    fun tokenStore(): TokenStore {
        return JwtTokenStore(accessTokenConverter())
    }

    override fun configure(configurer: ClientDetailsServiceConfigurer) {
        configurer.inMemory()
                .withClient("QTDesktop")
                .resourceIds("resource-server")
                .secret(oauthClientPasswordEncoder.encode("2EfZelLLf7fSvXOuUd6o"))
                .scopes("read", "write")
                .authorizedGrantTypes("password", "refresh_token")
                .accessTokenValiditySeconds(86400)
    }

    override fun configure(endpoints: AuthorizationServerEndpointsConfigurer) {
        val tokenEnhancerChain = TokenEnhancerChain()
        tokenEnhancerChain.setTokenEnhancers(listOf(tokenEnhancer(), accessTokenConverter()))

        endpoints.tokenStore(tokenStore())
                .tokenEnhancer(tokenEnhancerChain)
                .authenticationManager(authenticationManager)
//                .exceptionTranslator(exceptionTranslator())
    }

    @Bean
    fun tokenEnhancer(): BaseTokenEnhancer = TokenEnhancer()

    override fun configure(oauthServer: AuthorizationServerSecurityConfigurer) {
        oauthServer.tokenKeyAccess("permitAll()").checkTokenAccess("isAuthenticated()")
                .passwordEncoder(oauthClientPasswordEncoder)
    }

    @Bean
    @Primary
    fun tokenServices(): DefaultTokenServices {
        val defaultTokenServices = DefaultTokenServices()
        defaultTokenServices.setTokenStore(tokenStore())
        defaultTokenServices.setSupportRefreshToken(true)
        return defaultTokenServices
    }

//    @Bean
//    fun exceptionTranslator() : WebResponseExceptionTranslator<OAuth2Exception> {
//        return ExceptionTranslator()
//    }
}