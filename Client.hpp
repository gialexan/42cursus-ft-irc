/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:19:06 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/11 16:10:12 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()

//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

class Client //-> class for client
{
    public:
        Client();
        
        int get_fd() const;
        std::string get_buffer() const;
        std::string get_nickname() const;
        std::string get_username() const;
        std::string get_password() const;

        void set_fd(const int fd);
        void set_ip_add(const std::string &ipadd);
        void set_buffer(const std::string &buffer);
        void set_nickname(const std::string &nickname);
        void set_username(const std::string &username);
        void set_password(const std::string &password);

    private:
        int _fd; //-> client file descriptor
        std::string _ip_addr; //-> client ip address
        std::string _nickname; //-> client nickname
        std::string _username; //-> client username
        std::string _password; //-> client password
        std::string _buffer; //-> client buffer
        bool _is_registered; //-> boolean for registration
        bool _is_authenticated; //-> boolean for authentication
        bool _is_logged; //-> boolean for login
};