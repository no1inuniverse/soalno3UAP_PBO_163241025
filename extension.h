#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

class ParkingException : public std::exception {
protected:
    std::string message;
public:
    ParkingException(const std::string& msg);
    virtual const char* what() const noexcept override;
    virtual ~ParkingException();
};

class NoAvailableSlotException : public ParkingException {
public:
    NoAvailableSlotException();
};

class InvalidTicketException : public ParkingException {
public:
    InvalidTicketException();
};

class ReservationException : public ParkingException {
public:
    ReservationException();
};

class PaymentException : public ParkingException {
public:
    PaymentException();
};

#endif
