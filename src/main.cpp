#include <QApplication>
#include "controller/login_controller.hpp"
#include "controller/main_window_controller.hpp"
#include "model/token.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

//    (new LoginController)->show();

    Token token;
    token.accessToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOlsicmVzb3VyY2Utc2VydmVyIl0sIlVzZXJEZXRhaWxzRGF0YSI6eyJpZCI6NiwidXNlcm5hbWUiOiJhbmFAYW5hIiwibmFtZSI6Ik1hcmlhbiBSYXVsIEZsb3JpbiIsInJvbGVzIjpbIlVzZXIiXSwiYWN0aXZhdGVkIjp0cnVlfSwidXNlcl9uYW1lIjoiYW5hQGFuYSIsInNjb3BlIjpbInJlYWQiLCJ3cml0ZSJdLCJleHAiOjE1OTM4MTU2ODQsImF1dGhvcml0aWVzIjpbIlJPTEVfVVNFUiJdLCJqdGkiOiJjNTMwOTg3Zi03ZGFhLTRiNTktODNmNi1hMjA5NDU0Y2ExMGUiLCJjbGllbnRfaWQiOiJRVERlc2t0b3AifQ.4U7fvLnnB6mAo_jM5fBpyCWg-Z6VdpvZ9t8WiuVP3XY";
    TokenStorage::instance().saveToken(token);
    (new MainWindowController)->show();

    return QApplication::exec();
}
