
#import "RSSCycloneSettings.h"

NSString * const RSSCyclone_Settings_CyclonesCount=@"Cyclones";
NSString * const RSSCyclone_Settings_ParticlesCount=@"Particles";
NSString * const RSSCyclone_Settings_Size=@"Size";
NSString * const RSSCyclone_Settings_Speed=@"Speed";
NSString * const RSSCyclone_Settings_Complexity=@"Complexity";
NSString * const RSSCyclone_Settings_Stretch=@"Stretch";
NSString * const RSSCyclone_Settings_ShowCurves=@"Curve";

@implementation RSSCycloneSettings

- (id)initWithDictionaryRepresentation:(NSDictionary *)inDictionary
{
	self=[super init];
	
	if (self!=nil)
	{
		NSNumber * tNumber=inDictionary[RSSCyclone_Settings_CyclonesCount];
		
		if (tNumber==nil)
		{
			[self resetSettings];
		}
		else
		{
			_numberOfCyclones=[inDictionary[RSSCyclone_Settings_CyclonesCount] unsignedIntegerValue];
			_cycloneComplexity=[inDictionary[RSSCyclone_Settings_Complexity] unsignedIntegerValue];
			
			_numberOfParticlesPerCyclone=[inDictionary[RSSCyclone_Settings_ParticlesCount] unsignedIntegerValue];
			_particleSize=[inDictionary[RSSCyclone_Settings_Size] unsignedIntegerValue];
			
			_speed=[inDictionary[RSSCyclone_Settings_Speed] unsignedIntegerValue];
			
			_stretch=[inDictionary[RSSCyclone_Settings_Stretch] boolValue];
			_showCurves=[inDictionary[RSSCyclone_Settings_ShowCurves] boolValue];
		}
	}
	
	return self;
}

- (NSDictionary *)dictionaryRepresentation
{
	NSMutableDictionary * tMutableDictionary=[NSMutableDictionary dictionary];
	
	if (tMutableDictionary!=nil)
	{
		tMutableDictionary[RSSCyclone_Settings_CyclonesCount]=@(_numberOfCyclones);
		tMutableDictionary[RSSCyclone_Settings_Complexity]=@(_cycloneComplexity);
		
		tMutableDictionary[RSSCyclone_Settings_ParticlesCount]=@(_numberOfParticlesPerCyclone);
		tMutableDictionary[RSSCyclone_Settings_Size]=@(_particleSize);
		
		tMutableDictionary[RSSCyclone_Settings_Speed]=@(_speed);
		
		tMutableDictionary[RSSCyclone_Settings_Stretch]=@(_stretch);
		tMutableDictionary[RSSCyclone_Settings_ShowCurves]=@(_showCurves);
	}
	
	return [tMutableDictionary copy];
}

#pragma mark -

- (void)resetSettings
{
	_numberOfCyclones=1;
	_cycloneComplexity=3;
	
	_numberOfParticlesPerCyclone=200;
	_particleSize=7;
	
	_speed=10;
	
	_stretch=YES;
	_showCurves=NO;
}

@end
