package com.raul.licenta.configuration

import com.raul.licenta.model.Role
import org.springframework.context.annotation.Configuration
import org.springframework.security.config.annotation.web.builders.HttpSecurity
import org.springframework.security.oauth2.config.annotation.web.configuration.EnableResourceServer
import org.springframework.security.oauth2.config.annotation.web.configuration.ResourceServerConfigurerAdapter
import org.springframework.security.oauth2.config.annotation.web.configurers.ResourceServerSecurityConfigurer

@Configuration
@EnableResourceServer
class ResourceServerConfig : ResourceServerConfigurerAdapter() {
    override fun configure(resources: ResourceServerSecurityConfigurer) {
        resources.resourceId("resource-server")
    }

    override fun configure(http: HttpSecurity) {
        http
                .authorizeRequests()
                .antMatchers("/", "/webjars/**", "/unauthenticated/**")
                .permitAll()
                .antMatchers("/authenticated/**")
                .hasRole(Role.User.toRoleName())
                .antMatchers("/admin/**")
                .hasRole(Role.Admin.toRoleName())
                .anyRequest()
                .denyAll()
                .and()
                .logout()
                .permitAll()
                .and()
                .csrf()
                .disable()
    }
}