//
// Create a product in the output file.
// The product is a simple integer with the structure:
//
//    i = 1000*A + E
// where A={ 1:Tracker, 2:Calorimeter, 3:CRV)
//       E= event number
//
// Original author Rob Kutschke
//

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "fhiclcpp/types/Sequence.h"

#include <iostream>
#include <vector>

using namespace std;

namespace mu2e {

  class ToyProducer : public art::EDProducer {
  public:

    struct Config {
      fhicl::Atom<int> type {
        fhicl::Name("type"),
          fhicl::Comment("The InputTag of the StatusG4 object to analyze.")
          };
    };

    using Parameters = art::EDProducer::Table<Config>;
    explicit ToyProducer(const Parameters& conf);

    void produce( art::Event& e) override;

  private:

    int _type;

  };

  ToyProducer::ToyProducer(const Parameters& conf):
    art::EDProducer{conf},
    _type(conf().type())
  {
    std::cout << "Will produce type:  " << _type << std::endl;
    produces<int>();
  }

  void ToyProducer::produce(art::Event& event) {

    const int val = 10000*_type + event.id().event();

    unique_ptr<int> prod = std::make_unique<int>(val);
    event.put(std::move(prod));

  } // end ToyProducer::produce

}  // end namespace mu2e

DEFINE_ART_MODULE(mu2e::ToyProducer)
