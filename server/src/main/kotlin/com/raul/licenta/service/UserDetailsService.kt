package com.raul.licenta.service

import com.raul.licenta.model.*
import com.raul.licenta.repository.UserRepository
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.stereotype.Service
import org.springframework.transaction.annotation.Transactional
import java.lang.Exception

typealias BaseUserDetailsService = org.springframework.security.core.userdetails.UserDetailsService

@Service
class UserDetailsService(
        @Autowired
        val userRepository: UserRepository
) : BaseUserDetailsService {

    @Transactional
    override fun loadUserByUsername(username: String?): BaseUserDetails? {
        val notNullUsername: String = username ?: throw Exception("No user with the provided username was found!")
        val user: User = userRepository.findByUsername(notNullUsername).orElseThrow{
            Exception("No user with the provided username was found!")
        }
        return createUserDetails(user)
    }

    fun createUserDetails(user: User): UserDetails = UserDetails(UserDetailsData(
            user.id,
            user.username,
            user.name,
            getUserRoles(user),
            isActivated(user)),
            user.bcrypPassword)

    private fun getUserRoles(user: User): List<Role> {
        val roles: ArrayList<Role> = ArrayList()
        for (role in user.userRoles) {
            if (!roles.contains(Role.User) && role.role in Role.equivalentRoles) {
                roles.add(Role.User)
            } else if (role.role !in Role.equivalentRoles) {
                roles.add(role.role)
            }
        }
        return roles
    }

    private fun isActivated(user: User): Boolean = user.activation.activated
            && !(user.userRoles.size == 1 && user.userRoles.first().role == Role.Unassigned)
}