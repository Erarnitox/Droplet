#include <Poco/Logger.h>

#include <AuthUtils.hpp>
#include <EmailClient.hpp>
#include <UserManager.hpp>
#include <WebUserDTO.hpp>
#include <WebUserRepository.hpp>

#include "Poco/UUIDGenerator.h"
#include "Secrets.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
UserManager::UserManager() {
	const Secrets& sec{Secrets::getInstance()};
	smtpServer = sec.getSecret("smtp_server");
	smtpUser = sec.getSecret("smtp_user");
	smtpPassword = sec.getSecret("smtp_password");
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void UserManager::addUser(const std::string& email, const std::string& username, const std::string& password) {
	std::lock_guard<Poco::Mutex> lock(mutex);
	WebUserRepository repo;

	if (not repo.exists(username)) {
		WebUserDTO user;
		user.username = username;
		user.email = email;
		user.password = hashPassword(password);
		user.clearance = AuthClearance::PUBLIC;
		user.confirm_code = Poco::UUIDGenerator().createRandom().toString();
		user.is_verified = false;

		if (repo.create(user)) {
			sendVerificationEmail(email, user.confirm_code);
		}
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool UserManager::verifyUser(const std::string& token) {
	std::lock_guard<Poco::Mutex> lock(mutex);
	WebUserRepository repo;
	return repo.verify(token, AuthClearance::PRIVATE);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void UserManager::sendVerificationEmail(const std::string& email, const std::string& token) {
	try {
		EmailClient client(smtpServer, 465, smtpUser, smtpPassword);
		client.send(smtpUser,
					{email},
					"Email Verification",
					"To complete your registration, please click on the link below.\n"
					"If you did not register on erarnitox.de, you can ignore this e-mail.\n\n"
					"Verification Link:\nhttps://erarnitox.de/verify?token=" +
						token);
	} catch (const std::exception& ex) {
		std::cerr << "STD error: " << ex.what() << "\n";
		throw;
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
UserManager& UserManager::getInstance() noexcept {
	static UserManager instance;
	return instance;
}