/*
 * Trader.cpp
 */

// C++
#include <stdexcept>

// STL
#include <iostream>
#include <string>

#include "Price.hpp"
#include "LongPosition.hpp"
#include "ShortPosition.hpp"
#include "Trader.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::multi_index;


Trader::Trader(void):
  _pid(0)
{
}


// Buy a new position
Position::ID Trader::buy(const string& symbol, const date& dt, const Price& price, unsigned size) throw(TraderException)
{
  // Create new position
  Position* pPos;

  // Buy position
  try {

	pPos = new LongPosition(++_pid, symbol, dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  // Add new position to trader collection
  if( _miPositions.insert(pPos).first == _miPositions.end() )
	throw TraderException("Can't add position");

  // Return new position ID
  return _pid;
}


// Add buy for an existing position
void Trader::buy(Position::ID id, const boost::gregorian::date& dt, const Price& price, unsigned size) throw(TraderException)
{
  // Find existing position
  PositionSet::const_iterator iter = _miPositions.find(id, pos_comp_id());
  if( iter == _miPositions.end() )
	throw TraderException("Can't find position");

  // Add buy transaction to existing position
  Position* pPos = *iter;
  try {

	pPos->buy(dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  // Update existing position
  if( _miPositions.replace(iter, pPos) == false )
	throw TraderException("Can't update position");
}


// Sell an existing long position
void Trader::sell(Position::ID id, const date& dt, const Price& price, unsigned size) throw(TraderException)
{
  // Find existing position
  PositionSet::const_iterator iter = _miPositions.find(id, pos_comp_id());
  if( iter == _miPositions.end() )
	throw TraderException("Can't find position");

  // Add sell transaction to existing position
  Position* pPos = *iter;
  try {

	pPos->sell(dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  // Update existing position
  if( _miPositions.replace(iter, pPos) == false )
	throw TraderException("Can't update position");
}


Position::ID Trader::sell_short(const string& symbol, const date& dt, const Price& price, unsigned size) throw(TraderException)
{
  Position* pPos; 

 try {

	pPos = new ShortPosition(++_pid, symbol, dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  if( _miPositions.insert(pPos).first == _miPositions.end() )
	throw TraderException("Can't add position");

  return _pid;
}


void Trader::sell_short(Position::ID id, const date& dt, const Price& price, unsigned size) throw(TraderException)
{
  // Find existing position
  PositionSet::const_iterator iter = _miPositions.find(id, pos_comp_id());
  if( iter == _miPositions.end() )
	throw TraderException("Can't find position");

  // Add short execution to existing position
  Position *pPos = *iter;
  try {

	pPos->sell_short(dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  if( _miPositions.replace(iter, pPos) == false )
	throw TraderException("Can't update position");
}


void Trader::cover(Position::ID id, const date& dt, const Price& price, unsigned size) throw(TraderException)
{
  // Find existing position
  PositionSet::const_iterator iter = _miPositions.find(id, pos_comp_id());
  if( iter == _miPositions.end() )
	throw TraderException("Can't find position");

  // Add cover execution to existing position
  Position* pPos = *iter;
  try {

	pPos->cover(dt, price, size);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  // Update existing position
  if( _miPositions.replace(iter, pPos) == false )
	throw TraderException("Can't update position");
}


void Trader::close(Position::ID id, const date& dt, const Price& price) throw(TraderException)
{
  // Find existing position
  PositionSet::const_iterator iter = _miPositions.find(id, pos_comp_id());
  if( iter == _miPositions.end() )
	throw TraderException("Can't find position");

  // Close position
  Position* pPos = *iter;
  try {

	pPos->close(dt, price);

  } catch( const PositionException& e ) {
	ostringstream oss;
	oss << "Position error: " << e.what();
	throw TraderException(oss.str());

  } catch( const std::exception& e ) {
	ostringstream oss;
	oss << "Standard exception: " << e.what();
	throw TraderException(oss.str());
  }

  // Update existing position
  if( _miPositions.replace(iter, pPos) == false )
	throw TraderException("Can't update position");
}
