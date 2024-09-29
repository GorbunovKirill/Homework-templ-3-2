#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observable {
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void warning(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onFatalError(message);
        }
    }
};

class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};


class FileErrorObserver : public Observer {
    std::string filepath_;
public:
    FileErrorObserver(const std::string& filepath) : filepath_(filepath) {}

    void onError(const std::string& message) override {
        std::ofstream outfile(filepath_, std::ios::app);
        if (outfile.is_open()) {
            outfile << "Error: " << message << std::endl;
            outfile.close();
        }
        else {
            std::cerr << "Failed to open file: " << filepath_ << std::endl;
        }
    }
};

class ConsoleAndFileFatalErrorObserver : public Observer {
    std::string filepath_;
public:
    ConsoleAndFileFatalErrorObserver(const std::string& filepath) : filepath_(filepath) {}

    void onFatalError(const std::string& message) override {
        std::cout << "Fatal Error: " << message << std::endl;

        std::ofstream outfile(filepath_, std::ios::app);
        if (outfile.is_open()) {
            outfile << "Fatal Error: " << message << std::endl;
            outfile.close();
        }
        else {
            std::cerr << "Failed to open file: " << filepath_ << std::endl;
        }
    }
};

int main() {
    Observable logger;

    ConsoleWarningObserver consoleWarningObserver;
    FileErrorObserver fileErrorObserver("error_log.txt");
    ConsoleAndFileFatalErrorObserver fatalErrorObserver("fatal_error_log.txt");

    logger.addObserver(&consoleWarningObserver);
    logger.addObserver(&fileErrorObserver);
    logger.addObserver(&fatalErrorObserver);

    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.fatalError("This is a fatal error message.");

    return 0;
}
