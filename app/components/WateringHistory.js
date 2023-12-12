import React, { useState } from 'react';
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';


const WateringHistoryChart = ({ waterHistory }) => {
    const initialCount = 3;
    const [visibleCount, setVisibleCount] = useState(initialCount);

    const loadMore = () => {
        setVisibleCount(prevCount => prevCount + 5);
      };

    const loadLess = () => {
        setVisibleCount(prevCount => {
          const newCount = prevCount - 3;
          return newCount < initialCount ? initialCount : newCount;
        });
      };

const renderEntries = () => {
    if (!waterHistory) {
        return <Text>Loading...</Text>;
    }

    const entries = [];
    let lastDate = null;
    let lastHour = null;

    waterHistory.slice(0, visibleCount).forEach((timestamp, index) => {
      const [date, time] = timestamp.split(' ');
      const [hour] = time.split(':');

      // Check for new day
      if (date !== lastDate) {

        if (lastDate !== null) {
          entries.push(<View key={`separator-day-${date}`} style={styles.daySeparator} />);
        }
        entries.push(<Text key={`day-${date}`} style={styles.separatorDay}>New Day: {date}</Text>);
        lastDate = date;
        lastHour = null; // Reset hour for new day
      }

      // Check for new hour
      if (hour !== lastHour) {
        entries.push(<Text key={`hour-${date}-${hour}`} style={styles.separatorHour}>New Hour: {hour}:00:00</Text>);
        lastHour = hour;
      }

      entries.push(<Text key={`entry-${index}`} style={styles.entry}>{timestamp}</Text>);
    });

    return entries;
  };

    return (
        <View style={styles.container}>
            {renderEntries()}
            <View style={styles.buttonContainer}>
                {visibleCount > initialCount ? (
                <TouchableOpacity onPress={loadLess} style={styles.lessButton}>
                    <Text style={styles.buttonText}>Less</Text>
                </TouchableOpacity>
                ) : (
                <View style={styles.placeholder} />  // Invisible placeholder
                )}
                {visibleCount < waterHistory.length ? (
                <TouchableOpacity onPress={loadMore} style={styles.moreButton}>
                    <Text style={styles.buttonText}>More</Text>
                </TouchableOpacity>
                ) : (
                <View style={styles.placeholder} />  // Invisible placeholder
                )}
            </View>
        </View>
    );
};

const styles = StyleSheet.create({
    container: {
        backgroundColor: 'white',
        borderRadius: 10,
    },

    separatorDay: {
        fontWeight: 'bold',
        padding: 5,
        color: 'blue',
    },

    daySeparator: {
        height: 1,
        backgroundColor: 'black',
        width: '90%',
        marginVertical: 5,
        alignSelf:'center',
      },

      dashedLine: {
        borderBottomWidth: 1,
        borderColor: 'grey',
        borderStyle: 'dashed',
        marginVertical: 5,
      },

    separatorHour: {
        fontWeight: 'bold',
        marginTop: 5,
        marginLeft: 10,
      },

    entry: {
      paddingLeft: 18,
    },


    buttonText: {
        fontSize: 14,
        fontWeight: 'bold',
        color: 'white',
        alignSelf: 'center',
    },

    buttonContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        margin: 10,
    },

    moreButton: {
        width: 50,
        height: 22,
        borderRadius: 8,
        backgroundColor: '#4CAF50',
        justifyContent: 'center',
    },


    lessButton: {
        width: 50,
        height: 22,
        borderRadius: 8,
        backgroundColor: 'grey',
        justifyContent: 'center',
    },

  });

export default WateringHistoryChart;