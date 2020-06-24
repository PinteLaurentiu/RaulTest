package com.raul.licenta.repository

import com.raul.licenta.model.User
import com.raul.licenta.model.UserRole
import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.stereotype.Repository

@Repository
interface UserRoleRepository : JpaRepository<UserRole, Long> {
    fun deleteByUser(user: User)
}