import React, { useState, useEffect, useRef } from 'react';
import { View, Text, TouchableOpacity, StyleSheet, Dimensions, ScrollView } from 'react-native';
import moment from 'moment';
import { LineChart } from 'react-native-chart-kit';


const processDataForInterval = (dataLogs, startTime) => {
  const intervalData = [];

  let currentTime = moment(startTime);

  for (let i = 0; i < 4; i++) {
    const endTimme = currentTime.clone().add(30, 'minutes');
    const intervalLogs = dataLogs.filter(log => 
      moment(log.timestamp).isBetween(currentTime, endTimme)
    );  

    const averageSunlight = intervalLogs.reduce((total, log) => total + log.value, 0) / intervalLogs.length;
    intervalData.push(averageSunlight || 0);

    currentTime = endTimme;
  }
  return intervalData;
};

const generateLabels = (startMoment) => {
  let labels = [];
  for (let i = 0; i < 4; i++) { 
    labels.push(startMoment.clone().add(30 * i, 'minutes').format('h:mm A'));
  }
  return labels;
};

const calculateRangeAndFormatTime = (dataLogs, startTime) => {
  let formattedTimeRange = '';
  let minSunlight = Infinity;
  let maxSunlight = -Infinity; 

  dataLogs.forEach(log => {
    const timestamp = moment(log.timestamp);
    if (timestamp.isBetween(startTime, startTime.clone().add(2, 'hours'))) {
      minSunlight = Math.min(minSunlight, log.value);
      maxSunlight = Math.max(maxSunlight, log.value);
  }});

  const now = moment();
  let timeLabel = startTime.format('MMM D, h A');
  if (startTime.isSame(now, 'day')) {
    timeLabel = "Today";
  } else if (startTime.isSame(now.clone().subtract(1, 'days'), 'day')) {
    timeLabel = "Yesterday";
  } else {
    timeLabel = startTime.format('MMM D, h A');
  }

  if (timeLabel === "Today" || timeLabel === "Yesterday") {
    formattedTimeRange = `${timeLabel}, ${startTime.format('h A')} - ${startTime.clone().add(2, 'hours').format('h:mm A')}`;
  } else {
    formattedTimeRange = `${timeLabel} - ${startTime.clone().add(2, 'hours').format('h:mm A')}`;
  }
  
  return {
    range: minSunlight === Infinity ? "No Data" : `${minSunlight}-${maxSunlight}`,
    timeRange: formattedTimeRange
  };
};



const Graph = ({ levelLogs }) => {
  const [currentWindowStart, setCurrentWindowStart] = useState(moment().startOf('hour').subtract(2, 'hours'));
  const screenWidth = Dimensions.get('window').width;


  const { range, timeRange } = calculateRangeAndFormatTime(levelLogs, currentWindowStart);

  const graphData = {
    labels: generateLabels(currentWindowStart),
    datasets: [{
      data: processDataForInterval(levelLogs, currentWindowStart)
    }]
  };
 
  let touchStartX = 0;
  let touchEndX = 0;

  const handleSwipeGesture = () => {
    if (touchEndX < touchStartX) {
      setCurrentWindowStart(prev => prev.clone().add(2, 'hours'));
    } else if (touchEndX > touchStartX) {
      setCurrentWindowStart(prev => prev.clone().subtract(2, 'hours'));
    }
  };


 
  const chartConfig = {
        backgroundColor: '#ffffff',
        backgroundGradientFrom: '#fff',
        backgroundGradientTo: '#fff',
        decimalPlaces: 0, 
        color: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`, // color for the line
        labelColor: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`, // color for the labels
        style: {
          borderRadius: 16,
          
        },
        
        propsForDots: {
          r: '5',
          strokeWidth: '0',
          stroke: '#ffa726',
          fill: '#ffa726',
        },

        propsForBackgroundLines: {
          strokeDasharray: '0', // solid background lines
          stroke: '#e3e3e3', // color of the background lines
        },
      };
      

    return (
      <View
        onStartShouldSetResponder={() => true}
        onMoveShouldSetResponder={() => true}
        onResponderGrant={(evt) => {
        touchStartX = evt.nativeEvent.pageX;
      }}
      onResponderRelease={(evt) => {
        touchEndX = evt.nativeEvent.pageX;
        handleSwipeGesture();
      }}
      style={{ width: screenWidth }}
      >
      
      <View style={styles.textArea}>
        <Text style={styles.label}>RANGE</Text>
        <Text style={styles.rangeValue}>{range}</Text>
        <Text style={styles.timeRange}>{timeRange}</Text>
      </View>
    
      <LineChart
        data={graphData}
        width={screenWidth}
        height={220}
        chartConfig={chartConfig}
        bezier
      />
    </View>


    );
};

const styles = StyleSheet.create({
  textArea: {
    backgroundColor: 'white',
    padding: 10,
  },

    container: {
      flex: 1,
      paddingVertical:20,
    },

    label: {
      color: 'gray', 
      fontSize: 12,
      fontWeight: 'bold',
    },
    rangeValue: {
      color: 'black',
      fontSize: 16,
      fontWeight: 'bold',
    },
    timeRange: {
      color: 'gray',
      fontSize: 12,
      fontWeight: 'bold',
    },
  });


export default Graph;
