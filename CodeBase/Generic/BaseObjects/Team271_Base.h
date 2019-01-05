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

#ifndef TEAM271_BASE_H_
#define TEAM271_BASE_H_

#define T271_BASE_MAX_CHILDREN   (8U)

class t271_base_data_index : public t271_network_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_BASE_DATA_CHILDREN = t271_network_data_index::T271_DATA_COUNT,

      T271_DATA_COUNT,
   } t271_base_data_index_t;
};

class Team271Base : public Team271NetworkObject
{
protected:
   Team271Base* Parent_ = nullptr;
   Team271Base* Children_[T271_BASE_MAX_CHILDREN];

public:
   /*
    *
    * Constructor/Destructor
    *
    */

   Team271Base( Team271Base* const argParent = nullptr,
                const string& argName = "",
                const uint8_t argMessageCount = t271_base_data_index::T271_DATA_COUNT );

   virtual ~Team271Base( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   Team271Base* GetParent( void ) const;

   const string GetPath( void ) const;

   const string GetPath( const string& argClassName ) const;

   uint32_t GetDirty( void ) const override;

   uint32_t GetDirtyString( const uint32_t argIndex ) const override;

   uint32_t GetDirty( const uint32_t argIndex ) const override;

   /*
    *
    * Setters
    *
    */
   void SetParent( Team271Base* const argParent );

   void SetDirty( void ) override;

   void ClearDirty( void ) override;

   /*
    *
    * Base
    *
    */
   virtual void Init( void );

   /*
    *
    * Robot
    *
    */
   virtual void RobotInit( const bool argIsParent = false );

   virtual void PreRobotPeriodic( const bool argIsParent = false );

   virtual void PostRobotPeriodic( const bool argIsParent = false );

   /*
    *
    * Disabled
    *
    */
   virtual void DisabledInit( const bool argIsParent = false );

   virtual void DisabledPeriodic( const bool argIsParent = false );

   /*
    *
    * Autonomous
    *
    */
   virtual void AutonomousInit( const bool argIsParent = false );

   virtual void AutonomousPeriodic( const bool argIsParent = false );

   /*
    *
    * Teleop
    *
    */
   virtual void TeleopInit( const bool argIsParent = false );

   virtual void TeleopPeriodic( const bool argIsParent = false );

   /*
    *
    * Test
    *
    */
   virtual void TestInit( const bool argIsParent = false );

   virtual void TestPeriodic( const bool argIsParent = false );

   /*
    *
    * Robot State
    *
    */
   virtual void UpdateInput( const bool argIsParent = false );

   virtual void ClearStickyFaults( const bool argIsParent = false );

   virtual void SetDSO( const TEAM271_DSO_STATE argIsDSO,
                        const bool argIsParent = false );

   virtual void PIDControllerState( const uint32_t argEnable,
                                    const bool argIsParent = false );

   /*
    *
    * Child Functions
    *
    */
   int32_t ChildAdd( Team271Base* const argChild );
};

#endif /* TEAM271_BASE_H_ */
