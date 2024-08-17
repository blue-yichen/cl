//
// Created by yichen on 2024/8/14.
//

#ifndef CHATSERVER_INCLUDE_CONTROLMESSAGE_H_
#define CHATSERVER_INCLUDE_CONTROLMESSAGE_H_
#include <QString>
namespace ControlMessage{
enum ControlMessageEnum {
  GenerateAid,Forward,Login,Register,
  ForwardMessage,ForwardFile,AidRepeat,AidNoRepeat,
  LoginSuccessful,LoginError,Null,RegisterSuccessful,
  EndFlag,AidData,PasswordData,UsernameData
};
const  QString Mes[] = {
	"generate aid","forward",
	"login","register",
	"forward message","forward file",
	"repeat","no repeat","login successful",
	"login error"," ",
	"register successful","\n",
	"aid","password","username"
};
}


#endif //CHATSERVER_INCLUDE_CONTROLMESSAGE_H_
