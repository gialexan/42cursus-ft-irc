/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gilmar <gilmar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:30:47 by gilmar            #+#    #+#             */
/*   Updated: 2024/05/30 16:15:26 by gilmar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
 * Command: Kick 
 * Parameters: <channel> <user> [<comment>]
 * Reference: https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.8
*/

/*
 * Cenários de Teste:
 * 1. O comando KICK é recebido sem parâmetros suficientes.
 * 2. O comando KICK é recebido e o canal não existe.
 * 3. O comando KICK é recebido e o cliente não está no canal.
 * 4. O comando KICK é recebido e o cliente não é um operador do canal.
 * 5. O comando KICK é recebido e o cliente alvo não existe.
 * 6. O comando KICK é recebido e o cliente alvo não está no canal.
 * 7. O comando KICK é recebido e o cliente é expulso do canal com sucesso.
 * 8. O comando KICK é recebido e o cliente é expulso do canal com sucesso e um comentário é enviado.
 * 
*/

/**
 * @brief Handles the KICK command received from the client.
 *
 * This method processes the KICK command received from the client and sends a response
 * to the client indicating that the client has been kicked from the channel.
 *
 * @param buffer The buffer containing the KICK command parameters.
 * @param fd The file descriptor associated with the client that sent the command.
 */
void Server::_handler_client_kick(const std::string &buffer, const int fd)
{
	Client* client = _get_client(fd);

	if (!client->get_is_logged()) {
        _send_response(fd, ERR_NOTREGISTERED(client->get_nickname()));
        _reply_code = 451;
        return;
    }

    // Divide o buffer em parâmetros e verifica se há parâmetros suficientes
    std::vector<std::string> params = _split_buffer(buffer, SPACE);
    if (params.size() < 2) {
        _send_response(fd, ERR_NEEDMOREPARAMS(client->get_nickname()));
        _reply_code = 461;
        return;
    }

    // Extrai o nome do canal e o apelido do cliente a ser expulso
    std::string channel_name = params[0];
    std::vector<std::string> comments = _split_buffer(params[1], SPACE);
    std::string target_nickname = comments[0];

    // Verifica se o canal existe
    Channel *channel = _get_channel(channel_name);
    if (!channel) {
        _send_response(fd, ERR_NOSUCHCHANNEL(channel_name));
        _reply_code = 403;
        return;
    }

    // Verifica se o cliente está no canal
    if (!channel->has_client(client)) {
        _send_response(fd, ERR_NOTONCHANNEL(channel_name));
        _reply_code = 442;
        return;
    }

    // Verifica se o cliente é um operador do canal
    if (!channel->is_channel_operator(client->get_nickname())) {
        _send_response(fd, ERR_CHANOPRIVSNEEDED(channel_name));
        _reply_code = 482;
        return;
    }

    // Verifica se o cliente alvo existe
    Client *target_client = _get_client(target_nickname);
    if (!target_client) {
        _send_response(fd, ERR_NOSUCHNICK(channel_name, target_nickname));
        _reply_code = 401;
        return;
    }

    // Verifica se o cliente alvo está no canal
    if (!channel->has_client(target_client)) {
        _send_response(fd, ERR_USERNOTINCHANNEL(target_nickname, channel_name));
        _reply_code = 441;
        return;
    }

	// Check if option parameter <comment> exists
	if (params.size() > 2) {
		_send_response(fd, RPL_KICK(client->get_hostname(), channel_name, client->get_nickname(), target_client->get_nickname(), comments[1]));
	} else {
		_send_response(fd, RPL_KICK(client->get_hostname(), channel_name, client->get_nickname(), target_client->get_nickname(), ""));
	}

	channel->kick(target_client);
	_reply_code = 200;

    // Registra o comando KICK recebido
    std::cout << "KICK command received from client " << buffer << std::endl;
}
