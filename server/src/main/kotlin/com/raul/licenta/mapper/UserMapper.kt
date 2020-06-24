package com.raul.licenta.mapper

import com.raul.licenta.dto.RegisterDto
import com.raul.licenta.model.Activation
import com.raul.licenta.model.Role
import com.raul.licenta.model.User
import com.raul.licenta.model.UserRole
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.security.crypto.password.PasswordEncoder
import org.springframework.stereotype.Component

@Component
class UserMapper(
        @Autowired
        private val userPasswordEncoder: PasswordEncoder
) {
    fun getUser(dto: RegisterDto): User {
        val user = User(0,
                userPasswordEncoder.encode(dto.password),
                dto.email,
                dto.name,
                hashSetOf(UserRole(0, Role.Unassigned)),
                Activation(0, "", true))
        user.userRoles.first().user = user;
        user.activation.user = user
        return user
    }
}
