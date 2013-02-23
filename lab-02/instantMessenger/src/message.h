/*
 * message.h:	Header containing message types &
 *		definitions of the message structs
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#define NAME_MAX_LEN 256
#define MESSAGE_MAX_LEN 1024

/* Different types of messages used in the model. */
typedef enum im_message_type {
  REGISTRATION_MESSAGE,		// Registration message
  DEREGISTRATION_MESSAGE,	// Deregistration message
  INSTANT_MESSAGE		// General IM message
} im_message_type;

/* Message format to be used in case of all types of messages. */
typedef struct im_message {
  im_message_type type;		// message type
  char to[256];			// receiver's name
  char from[256];		// sender's name
  char message[1024];		// actual message
} im_message;
