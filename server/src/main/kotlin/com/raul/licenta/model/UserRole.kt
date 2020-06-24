package com.raul.licenta.model

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity(name = "userRole")
data class UserRole(
        @Id
        @GeneratedValue(strategy = GenerationType.IDENTITY)
        var id : Long = 0,

        @Column
        var role : Role = Role.User
) {
    @ManyToOne(optional = false)
    @JoinColumn(name = "user_id")
    @JsonIgnore
    lateinit var user: User
}