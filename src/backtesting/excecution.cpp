
#include "backtesting/excecution.hpp"
#include "core/types.hpp"
#include "data/config.hpp"

trd::price Excecution::slip(trd::price refPrice, trd::Side side) const {

    // Apply slippage per trade to simulate trades excecuting at a different price than expected resulting in pure loss 
    
    trd::price slipAmt = refPrice * SLIP_BPS / 10000.0;

    switch (side) {
        case trd::Side::Buy: return refPrice + slipAmt;
        case trd::Side::Sell: return refPrice - slipAmt;
        default: return refPrice;
    }

}
