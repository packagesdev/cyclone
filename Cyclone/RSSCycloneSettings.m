
#import "RSSCycloneSettings.h"

NSString * const RSSCyclone_Settings_CyclonesCountKey=@"Cyclones";
NSString * const RSSCyclone_Settings_ParticlesCountKey=@"Particles";
NSString * const RSSCyclone_Settings_SizeKey=@"Size";
NSString * const RSSCyclone_Settings_SpeedKey=@"Speed";
NSString * const RSSCyclone_Settings_ComplexityKey=@"Complexity";
NSString * const RSSCyclone_Settings_StretchKey=@"Stretch";
NSString * const RSSCyclone_Settings_ShowCurvesKey=@"Curve";

@implementation RSSCycloneSettings

- (id)initWithDictionaryRepresentation:(NSDictionary *)inDictionary
{
	self=[super init];
	
	if (self!=nil)
	{
		NSNumber * tNumber=inDictionary[RSSCyclone_Settings_CyclonesCountKey];
		
		if (tNumber==nil)
		{
			[self resetSettings];
		}
		else
		{
			_numberOfCyclones=[inDictionary[RSSCyclone_Settings_CyclonesCountKey] unsignedIntegerValue];
			_cycloneComplexity=[inDictionary[RSSCyclone_Settings_ComplexityKey] unsignedIntegerValue];
			
			_numberOfParticlesPerCyclone=[inDictionary[RSSCyclone_Settings_ParticlesCountKey] unsignedIntegerValue];
			_particleSize=[inDictionary[RSSCyclone_Settings_SizeKey] unsignedIntegerValue];
			
			_speed=[inDictionary[RSSCyclone_Settings_SpeedKey] unsignedIntegerValue];
			
			_stretch=[inDictionary[RSSCyclone_Settings_StretchKey] boolValue];
			_showCurves=[inDictionary[RSSCyclone_Settings_ShowCurvesKey] boolValue];
		}
	}
	
	return self;
}

- (NSDictionary *)dictionaryRepresentation
{
	NSMutableDictionary * tMutableDictionary=[NSMutableDictionary dictionary];
	
	if (tMutableDictionary!=nil)
	{
		tMutableDictionary[RSSCyclone_Settings_CyclonesCountKey]=@(_numberOfCyclones);
		tMutableDictionary[RSSCyclone_Settings_ComplexityKey]=@(_cycloneComplexity);
		
		tMutableDictionary[RSSCyclone_Settings_ParticlesCountKey]=@(_numberOfParticlesPerCyclone);
		tMutableDictionary[RSSCyclone_Settings_SizeKey]=@(_particleSize);
		
		tMutableDictionary[RSSCyclone_Settings_SpeedKey]=@(_speed);
		
		tMutableDictionary[RSSCyclone_Settings_StretchKey]=@(_stretch);
		tMutableDictionary[RSSCyclone_Settings_ShowCurvesKey]=@(_showCurves);
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
