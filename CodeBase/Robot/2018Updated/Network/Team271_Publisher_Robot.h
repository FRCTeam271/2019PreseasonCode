/*
 *    FRC Team 271's 2019 Pre Season Code
 *    Copyright (C) 2019  FRC Team 271
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TEAM271_PUBLISHER_ROBOT_H_
#define TEAM271_PUBLISHER_ROBOT_H_

class Team271PublisherRobot : public Team271NetworkPublisher
{
protected:

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271PublisherRobot( void );

   virtual ~Team271PublisherRobot( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   uint32_t GetDirty( void ) const override;

   uint32_t GetDirty( const uint32_t argIndex ) const override;

   /*
    *
    * Setters
    *
    */
   void SetDirty( void ) override;

   void ClearDirty( void ) override;

   /*
    *
    * Publisher
    *
    */
   void Pub( void ) override;
};

#endif /* TEAM271_PUBLISHER_ROBOT_H_ */
