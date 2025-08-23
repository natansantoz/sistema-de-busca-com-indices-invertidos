#include "Indice.h"
#include <iostream> 
#include <string.h>

using namespace std;

Indice::Indice(long long i, streampos o) : id(i), offset(o) {}

Indice::Indice() : id(-1), offset(-1) {}

long long Indice::getId() const {
    return id;
}

streampos Indice::getOffset() const {
    return offset;
}

bool Indice::operator>(const Indice& outro) const {
    return id > outro.id;
}

bool Indice::operator<(const Indice& outro) const {
    return id < outro.id;
}

bool Indice::operator==(const Indice& outro) const {
    return id == outro.id;
}

bool Indice::operator!=(const Indice& outro) const {
    return id != outro.id;
}

bool Indice::operator<=(const Indice& outro) const {
    return id <= outro.id;
}

bool Indice::operator>=(const Indice& outro) const {
    return id >= outro.id;
}

void Indice::setId(long long i) {
    id = i;
}

void Indice::setOffset(streampos o) {
    offset = o;
}

void Indice::exibir() const {
    cout << "ID: " << id << ", Offset: " << offset << endl;
}

string Indice::pack() const {
    char* buffer = new char[sizeof(long long) + sizeof(streampos)];

    memcpy(buffer, &this->id, sizeof(long long));
    memcpy(buffer + sizeof(long long), &this->offset, sizeof(streampos));

    string packed(buffer, sizeof(long long) + sizeof(streampos));

    delete[] buffer;

    return packed;
}

void Indice::unpack(ifstream& arquivoBinarioRegitros) {

    char* buffer = new char[sizeof(long long) + sizeof(streampos)];

    arquivoBinarioRegitros.read(buffer, sizeof(long long) + sizeof(streampos));
    
    memcpy(&this->id, buffer, sizeof(long long));
    memcpy(&this->offset, buffer + sizeof(long long), sizeof(streampos));

    delete[] buffer;
}

std::ostream& operator<<(std::ostream& os, const Indice& indice) {
    os << "ID: " << indice.id ;
    return os;
}
