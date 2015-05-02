
#import "RSSSettings.h"

@interface RSSCycloneSettings : RSSSettings

@property NSUInteger numberOfCyclones;
@property NSUInteger cycloneComplexity;

@property NSUInteger numberOfParticlesPerCyclone;
@property NSUInteger particleSize;

@property NSUInteger speed;

@property BOOL stretch;
@property BOOL showCurves;

@end
