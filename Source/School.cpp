#include "../Headers/School.h"

School::School() {

}

School::School(vector<Client> Clients, vector<Material> Materials) {
    this->Clients = Clients;
    this->Materials = Materials;
}
