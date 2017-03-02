#include "message.h"

#include <map>
callback_map_t callbacks;
static message_t createmessage(const std::string& name);

void invorkfun(const std::string& name, const std::string& data)
{
	auto msg = createmessage(name); 
    if (msg == NULL){
        return;
    }

    msg->ParseFromString(data);

    auto descriptor = msg->GetDescriptor();
    auto itor = callbacks.find(descriptor);

    if (itor != callbacks.end()){
        itor->second(msg);
    }else{
        printf("can not find the functional.\n");
    }
	// callback = std::bind(onUnknowMessage, this, std::placeholders::_1,std::placeholders::_2);
}
callback_t getfunc(const std::string& name)
{
	callback_t callback;
	const Descriptor * descriptor 
		= DescriptorPool::generated_pool()->FindMessageTypeByName(name);

	if (descriptor){
		    auto itor = callbacks.find(descriptor);
			if (itor != callbacks.end()){
				callback = itor->second;
		   } 
	}else{
        printf("parse msg error \n");
    }

	return callback;
}
/* Message declare in proto file，eg, in test.proto file:
   package mustduy;
   message image_t{
	   required int id;
	   required char[128] name;
   };
   \ name parameter is : package name + message name; --> mustudy.image_t, and so on.
*/
static message_t createmessage(const std::string& name){
	message_t message = NULL;
	const Descriptor * descriptor 
		= DescriptorPool::generated_pool()->FindMessageTypeByName(name);

	if (descriptor){
		const Message * prototype
			= MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype){
			message =  message_t(prototype->New());
		}
	} 

	return message;
}
/*
template<typename T>
void ProtoMessage::registerMessage(callback_t func)
{
   // auto descriptior = mstudy::ImageData::descriptor();
    auto descriptior = T::descriptor();
    callbacks[descriptior] = func;
}
*/

