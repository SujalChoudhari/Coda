#include "../../FFI/FFI.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

extern "C" EXPORT void coda_create_file(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string dirPath = args->getProperties()["dirPath"]->getValue();
	const std::string fileName = args->getProperties()["fileName"]->getValue();
	const std::string content = args->getProperties()["content"]->getValue();

	std::filesystem::path filePath = std::filesystem::path(dirPath) / fileName;
	std::ofstream outFile(filePath);
	res->setType(Coda::Runtime::Type::BOOL);

	if (outFile.is_open()) {
		outFile << content;
		outFile.close();
		res->setValue("1");
		return;
	}
	else {
		res->setValue("0");
		return;
	}
}


extern "C" EXPORT void coda_delete_file(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string filePath = args->getProperties()["filePath"]->getValue();

	// Use std::filesystem to delete the file
	if (std::filesystem::remove(filePath)) {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("1"); // File deletion success
	}
	else {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("0"); // File deletion failure
	}
}

extern "C" EXPORT void coda_create_directory(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string dirPath = args->getProperties()["dirPath"]->getValue();

	// Use std::filesystem to create the directory
	if (std::filesystem::create_directory(dirPath)) {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("1"); // Directory creation success
	}
	else {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("0"); // Directory creation failure
	}
}

extern "C" EXPORT void coda_delete_directory(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string dirPath = args->getProperties()["dirPath"]->getValue();

	// Use std::filesystem to delete the directory and its contents
	if (std::filesystem::remove_all(dirPath)) {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("1"); // Directory deletion success
	}
	else {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("0"); // Directory deletion failure
	}
}


extern "C" EXPORT void coda_file_exists(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string filePath = args->getProperties()["filePath"]->getValue();

	// Use std::filesystem to check if the file exists
	if (std::filesystem::exists(filePath)) {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("1"); // File exists
	}
	else {
		res->setType(Coda::Runtime::Type::BOOL);
		res->setValue("0"); // File does not exist
	}
}

extern "C" EXPORT void coda_rename(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string oldPath = args->getProperties()["oldPath"]->getValue();
	const std::string newPath = args->getProperties()["newPath"]->getValue();

	// Use std::filesystem to rename or move the file/directory
	res->setType(Coda::Runtime::Type::BOOL);
	try
	{
		std::filesystem::rename(oldPath, newPath);
		res->setValue("1"); // Rename/move success
	}
	catch (...) {
		res->setValue("0"); // Rename/move failed
	}

}


extern "C" EXPORT void coda_list_files(IValuePtr res, IValuePtr args, IEnvironment * env) {
	const std::string dirPath = args->getProperties()["dirPath"]->getValue();
	std::vector<std::string> fileList;

	// Use std::filesystem to list files and subdirectories
	for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
		fileList.push_back(entry.path().string());
	}

	auto resProps = res->getProperties();
	unsigned int count = 1;
	for (auto item : fileList) {
		resProps.insert({ std::to_string(count),res->createNewValue(Coda::Runtime::Type::STRING,item) });
		count++;
	}

	res->setType(Coda::Runtime::Type::LIST);
	res->setProperties(resProps);
}


