package com.raul.licenta.model

//import java.time.LocalDateTime
import com.fasterxml.jackson.annotation.JsonIgnore
import javax.persistence.*

@Entity(name = "activation")
data class Activation(
        @Id
        @GeneratedValue(strategy = GenerationType.IDENTITY)
        var id: Long = 0,

        @Column(nullable = false)
        var uuid: String = "",

        @Column(nullable = false)
        var activated: Boolean = false
) {
        @JsonIgnore
        @OneToOne(mappedBy = "activation")
        lateinit var user: User
}