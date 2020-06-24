package com.raul.licenta.model

import org.springframework.security.core.GrantedAuthority
import org.springframework.security.core.authority.SimpleGrantedAuthority

enum class Role {
    User,
    Admin,
    Generalist,
    Rezident,
    Specialist,
    Primar,
    Unassigned;

    fun toAuthority(): GrantedAuthority = SimpleGrantedAuthority("ROLE_" + toRoleName())

    fun toRoleName(): String = toString().toUpperCase()

    companion object {
        val equivalentRoles = listOf(User, Generalist, Rezident, Specialist, Primar)
    }
}

