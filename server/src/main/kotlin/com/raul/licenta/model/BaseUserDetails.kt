package com.raul.licenta.model

import org.springframework.security.core.CredentialsContainer
import org.springframework.security.core.GrantedAuthority

typealias BaseUserDetails = org.springframework.security.core.userdetails.UserDetails

data class UserDetailsData(
    var id: Long = 0,
    var username: String = "",
    var name: String = "",
    var roles: List<Role> = ArrayList(),
    var activated: Boolean = false
)

class UserDetails(
        var data : UserDetailsData,
        private var bCryptPassword : String? = null
) : BaseUserDetails, CredentialsContainer {

    override fun eraseCredentials() { bCryptPassword = null }

    override fun getAuthorities(): MutableCollection<out GrantedAuthority> =
            data.roles.map{ it.toAuthority() }.toMutableList()

    override fun isEnabled(): Boolean = data.activated

    override fun getUsername(): String = data.username

    override fun isCredentialsNonExpired(): Boolean = true

    override fun getPassword(): String = bCryptPassword.orEmpty()

    override fun isAccountNonExpired(): Boolean = true

    override fun isAccountNonLocked(): Boolean = true
}
