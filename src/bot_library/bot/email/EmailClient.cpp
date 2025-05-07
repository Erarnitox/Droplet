/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages Secrets
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#include <EmailClient.hpp>
#include <chrono>
#include <cstring>

#include "curl/curl.h"

//-----------------------------------------------------
//
//-----------------------------------------------------
namespace {

//-----------------------------------------------------
//
//-----------------------------------------------------
struct UploadContext {
	const std::string& payload;
	size_t pos;
};

//-----------------------------------------------------
//
//-----------------------------------------------------
size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp) {
	UploadContext* ctx = static_cast<UploadContext*>(userp);
	const size_t bytes_to_copy = std::min(size * nmemb, ctx->payload.size() - ctx->pos);
	if (bytes_to_copy > 0) {
		std::memcpy(ptr, ctx->payload.data() + ctx->pos, bytes_to_copy);
		ctx->pos += bytes_to_copy;
		return bytes_to_copy;
	}
	return 0;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::string convert_to_crlf(std::string_view sv) {
	std::string result;
	for (char c : sv) {
		if (c == '\n' && (result.empty() || result.back() != '\r')) {
			result += "\r\n";
		} else {
			result += c;
		}
	}
	return result;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::string build_payload(std::string_view from,
						  const std::vector<std::string>& to,
						  std::string_view subject,
						  std::string_view body) {
	std::ostringstream payload_ss;
	payload_ss << "From: <" << from << ">\r\n";
	payload_ss << "To: ";
	for (size_t i = 0; i < to.size(); ++i) {
		if (i > 0)
			payload_ss << ", ";
		payload_ss << "<" << to[i] << ">";
	}
	payload_ss << "\r\n";
	payload_ss << "Subject: " << subject << "\r\n";

	auto now = std::chrono::system_clock::now();
	auto time_t_now = std::chrono::system_clock::to_time_t(now);
	std::tm* tm_now = std::gmtime(&time_t_now);
	std::stringstream date_ss;
	date_ss.imbue(std::locale("C"));
	date_ss << std::put_time(tm_now, "%a, %d %b %Y %H:%M:%S +0000");
	payload_ss << "Date: " << date_ss.str() << "\r\n";

	payload_ss << "Content-Type: text/plain; charset=utf-8\r\n";
	payload_ss << "\r\n";
	payload_ss << convert_to_crlf(body);

	return payload_ss.str();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
class CurlSlist {
  public:
	CurlSlist() : slist_(nullptr) {
	}
	~CurlSlist() {
		curl_slist_free_all(slist_);
	}
	void append(const char* str) {
		curl_slist* new_slist = curl_slist_append(slist_, str);
		if (!new_slist) {
			throw std::runtime_error("Failed to append to curl_slist");
		}
		slist_ = new_slist;
	}
	curl_slist* get() {
		return slist_;
	}

  private:
	curl_slist* slist_;
};

//-----------------------------------------------------
//
//-----------------------------------------------------
struct CurlEasyDeleter {
	void operator()(CURL* curl) const {
		curl_easy_cleanup(curl);
	}
};

}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
EmailClient::EmailClient(const std::string& host,
						 uint16_t port,
						 const std::string& username,
						 const std::string& password)
	: _host{host}, _port{port}, _username{username}, _password{password} {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
EmailClient::~EmailClient() noexcept {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void EmailClient::send(std::string_view from,
					   const std::vector<std::string>& to,
					   std::string_view subject,
					   std::string_view body) {
	const std::string payload = build_payload(from, to, subject, body);
	std::unique_ptr<CURL, CurlEasyDeleter> curl(curl_easy_init());
	if (not curl) {
		throw std::runtime_error("Failed to initialize libcurl");
	}

	std::ostringstream url_ss;
	url_ss << "smtp://" << _host << ":" << _port;
	std::string url = url_ss.str();
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_USERNAME, _username.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, _password.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_FROM, from.data());

	CurlSlist recipients;
	for (const auto& recipient : to) {
		recipients.append(recipient.c_str());
	}
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_RCPT, recipients.get());

	UploadContext ctx{payload, 0};
	curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, payload_source);
	curl_easy_setopt(curl.get(), CURLOPT_READDATA, &ctx);
	curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

	CURLcode res = curl_easy_perform(curl.get());
	if (res != CURLE_OK) {
		throw std::runtime_error("Failed to send email: " + std::string(curl_easy_strerror(res)));
	}
}