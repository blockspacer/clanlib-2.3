
#include "precomp.h"
#include "server_login_events.h"
#include "server.h"

ServerLoginEvents::ServerLoginEvents(Server *server)
: server(server)
{
	login_events.func_event("login").set(this, &ServerLoginEvents::on_event_login);
}

ServerLoginEvents::~ServerLoginEvents()
{
}

void ServerLoginEvents::on_event_login(const CL_NetGameEvent &e, ServerPlayer *player)
{
	CL_String player_name = e.get_argument(0);

	server->login(player, player_name);
}
