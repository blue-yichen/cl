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
  EndFlag,AidData,PasswordData,UsernameData,
  RegisterError,SendAid,ReceiverAid,Delimiter,
  EndMessage,NoReadMessage,AddFriend,AnotherSideAid,
  AgreeFriend,HostAddFriendSuccess,PassiveAddFriendSuccess,
  RepeatAdd,AidNoExist,BeAdding,FriendApply,
  FriendFunc,RefuseFriend,BeFriendFail,ToAddFriendBeRefuse,
  TheOtherAddsFriendToo,TheOtherAppliedToBeFriend,RepeatApply,
  FriendIsSelf,RepeatLogin,AccountOrPasswordError,NoFriendRelation,
  LocalChatHistoryExist,TheHashValueOfLocalChatHistory,
  LocalChatHistoryNoExist,LocalChatHistoryIsRight,
  LocalChatHistoryIsNotRight,ChatHistoryNoExist,
  ChatHistoryExist,Origin,Timestamp,
};
const  QString Mes[] = {
	"generate aid","forward",
	"login","register",
	"forward message","forward file",
	"repeat","no repeat","login successful",
	"login error"," ",
	"register successful","\n",
	"aid","password","username","register error",
	"sender aid","receiver aid",
    ":","end",
    "no read message","add friend",
    "another side aid","agree to be friend",
    "host add friend ok","passive add friend ok",
    "repeat add",
    "aid no exist","be adding",
    "friend apply",
    "friend","refuse to be friend",
    "make friend fail","to add friend be refused",
    "the other adds friend,too","the other applied to be friend",
    "repeat apply","friend is yourself","repeat login",
    "account or password error","no friend relation",
    "local chat history exist",
    "the hash value of local chat history",
    "local chat history no exist",
    "local chat history is right",
    "local chat history is not right",
    "chat history no exist","chat history exist","origin",
	"timestamp",
};
}


#endif //CHATSERVER_INCLUDE_CONTROLMESSAGE_H_
