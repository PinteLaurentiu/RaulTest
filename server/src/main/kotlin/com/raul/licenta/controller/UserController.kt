package com.raul.licenta.controller

import com.raul.licenta.dto.RegisterDto
import com.raul.licenta.mapper.UserMapper
import com.raul.licenta.model.Role
import com.raul.licenta.service.UserService
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.PathVariable
import org.springframework.web.bind.annotation.PostMapping
import org.springframework.web.bind.annotation.RequestBody
import org.springframework.web.bind.annotation.RestController

@RestController
class UserController(
        @Autowired
        private val userService: UserService,

        @Autowired
        private val userMapper: UserMapper
) {
//    @GetMapping("/authenticated/")
//    fun auto() : ResponseEntity<List<User>> {
//        return ResponseEntity(userService.getAllUsers(), HttpStatus.OK)
//    }
    @PostMapping("/unauthenticated/user")
    fun register(@RequestBody dto: RegisterDto) {
        userService.register(userMapper.getUser(dto))
    }

    @PostMapping("/admin/user/{id}/{role}")
    fun changeRole(@PathVariable id: Long, @PathVariable role: Int) {
        if (role >= Role.values().size || role < 0) {
            throw Exception("Bad argument role")
        }
        userService.changeRole(id, Role.values()[role])
    }
}