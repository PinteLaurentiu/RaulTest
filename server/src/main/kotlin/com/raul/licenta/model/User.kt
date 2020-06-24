package com.raul.licenta.model

import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity(name = "user")
@Table(uniqueConstraints = [UniqueConstraint(columnNames = ["username"])])
data class User(
        @Id
        @GeneratedValue(strategy = GenerationType.IDENTITY)
        var id : Long = 0,

        @JsonIgnore
        @Column(nullable = true)
        var bcrypPassword : String? = null,

        @Column(nullable = false)
        var username: String = "",

        @Column(nullable = false)
        var name: String = "",

        @OneToMany(mappedBy = "user", cascade = [CascadeType.ALL])
        var userRoles: MutableSet<UserRole> = HashSet(),

        @OneToOne(cascade = [CascadeType.ALL])
        @JoinColumn(name = "activation_id", referencedColumnName = "id")
        var activation: Activation = Activation()
)