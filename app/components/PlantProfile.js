import React, { useEffect, useState } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, TextInput, Switch, TouchableOpacity, StyleSheet, Platform, Keyboard, TouchableWithoutFeedback } from 'react-native';
import axios from 'axios';
import WateringHistory from './WateringHistory';
import Graph from './Graph';


// import StarRating from "react-svg-star-rating";
// const healthActiveColor = "#70A327";
// const healthInactiveColor = "rgba(112, 163, 39, 0.5)";
// const unhealthActiveColor = "#FF0120";
// const unhealthInactiveColor = "rgba(255, 1, 32, 0.5)";

// const Star = ({ isActive }) => {
//   <Text style={[styles.star, { color: isActive ? activeColor : inactiveColor }]}>★</Text>
// }




const PlantProfile = (route) => {
    // const [rating, setRating] = useState(plant.health);

    // const activeColor = rating > 3 ? healthActiveColor : unhealthActiveColor;
    // const inactiveColor = rating > 3 ? healthInactiveColor : unhealthInactiveColor;
    const plant = route.route.params;

    const [plantInfo, setPlantInfo] = useState([]);
    const [batteryLevel, setBatteryLevel] = useState(null);
    const [moistureLevel, setMoistureLevel] = useState([]);
    const [sunlightLevel, setSunlightLevel] = useState([]);
    const [waterHistory, setWaterHistory] = useState([]);
    const [refreshing, setRefreshing] = useState(false);
    const fetchPlantInfo = async () => {
        try {
            const response = await axios.get(`https://fit-glowworm-promptly.ngrok-free.app/plants/${plant.plantId}`);
            setPlantInfo(response.data);
        } catch (error) {
            console.error('Error fetching plant info', error);
        }
    };

    useEffect(() => {
        fetchPlantInfo();
    }, []);

    // Battery Level
    useEffect(() => {
        if (plantInfo.sensorDataLogs && plantInfo.sensorDataLogs.length > 0) {
            const latestLog = plantInfo.sensorDataLogs[plantInfo.sensorDataLogs.length - 1];
            const batteryLevel = latestLog.deviceBattery;
            setBatteryLevel(batteryLevel);
        }
    }, [plantInfo.sensorDataLogs]);

    const getBatteryColor = (level) => {
        if (level < 20) return '#FFA07A';
        if (level < 60) return '#FFFF99';
        return '#90EE90';
    };

    // Fetch moisture level
    useEffect(() => {
        if (plantInfo.sensorDataLogs && plantInfo.sensorDataLogs.length > 0) {
            const moistureData = plantInfo.sensorDataLogs.map(log => ({
                timestamp: log.timestamp,
                value: log.soilMoisture,
            }));
            setMoistureLevel(moistureData);
        }
    }, [plantInfo.sensorDataLogs]);

    // Fetch sunlight level
    useEffect(() => {
        if (plantInfo.sensorDataLogs && plantInfo.sensorDataLogs.length > 0) {
            const sunlightData = plantInfo.sensorDataLogs.map(log => ({
                timestamp: log.timestamp,
                value: log.sunlight,
            }));
            setSunlightLevel(sunlightData);
        }
    }, [plantInfo.sensorDataLogs]);

    // Fetch water history
    useEffect(() => {
        if (plantInfo.waterHistory && plantInfo.waterHistory.length > 0) {
            const waterHistory = plantInfo.waterHistory;
            setWaterHistory(waterHistory);
        }
    }, [plantInfo.waterHistory]);



    return (
        <KeyboardAvoidingView
            behavior={Platform.OS === 'ios' ? 'padding' : "height"}
            style={styles.container}>
            <ScrollView refreshControl={
                <RefreshControl refreshing={refreshing} onRefresh={() => {
                    fetchPlantInfo();
                    setRefreshing(true);
                }} />}>

                <TouchableWithoutFeedback onPress={Keyboard.dismiss}>
                    <View style={styles.inner}>


                        <View style={styles.center}>
                            <Text style={styles.header}>{plant.plantName}</Text>
                            {/* <Image source={plant.image} style={styles.image}/> */}


                            {/* <View>
                        <Text style={styles.labelTitle}>Current Battery Level</Text>
                            {batteryLevel !== null ? (
                                <View style={[styles.battery, { backgroundColor: getBatteryColor(batteryLevel) }]}>
                                    <Text style={styles.batteryText}>{`${(batteryLevel)}%`}</Text>
                                </View>
                            ) : (
                                <Text>Fetching battery level...</Text>
                            )}

                        </View> */}
                            <View style={styles.batteryContainer}>
                                <Text style={styles.labelTitle}>Current Battery Level</Text>


                                <View style={{ flexDirection: 'row', alignItems: 'center' }}>
                                    <View style={styles.batteryHead} />
                                    <View style={styles.batteryBody}>
                                        <View
                                            style={[
                                                styles.batteryLevel,
                                                { width: `${batteryLevel}%`, backgroundColor: getBatteryColor(batteryLevel) }
                                            ]}
                                        />
                                        <Text style={styles.batteryText}>{`${batteryLevel}%`}</Text>
                                    </View>
                                </View>

                            </View>


                        </View>


                        {/* <View>
                        <Text style={styles.labelTitle}>Date</Text>
                        <Text style={styles.label}>{plant.date}</Text>
                    </View> */}

                        {/* <View>
                        <Text style={styles.labelTitle}>Additional Information</Text>
                        <Text style={styles.label}>{plant.additionalInfo}</Text>
                    </View> */}

                        {/* <View>
                        <Text style={styles.labelTitle}>Overall Health Status</Text>
                        <View style={styles.starsBar}>
                            <View style={styles.stars}>
                                {[...Array(5)].map((_, index) => (
                                    <Text
                                    key={index}
                                    style={{
                                        color: index < rating ? activeColor : inactiveColor,
                                        ...styles.star,
                                    }}
                                    >
                                    ★
                                    </Text>
                                ))}
                            </View>
                        </View>
                    </View> */}


                        <View>
                            <Text style={styles.label}>Soil Moisture Level</Text>
                            <Graph levelLogs={moistureLevel} />
                        </View>

                        <View>
                            <Text style={styles.label}>Sunlight Level</Text>
                            <Graph levelLogs={sunlightLevel} />
                        </View>

                        <View>
                            <Text style={styles.label}>Watering History</Text>
                            <WateringHistory waterHistory={waterHistory} />
                        </View>





                    </View>
                </TouchableWithoutFeedback>
            </ScrollView>
        </KeyboardAvoidingView >
    );
}

const styles = StyleSheet.create({

    container: {
        flex: 1,
        justifyContent: 'center',
        backgroundColor: '#ecf0f1',
        padding: 20,
    },

    header: {
        fontSize: 28,
        fontWeight: 'bold',
        marginBottom: 20,
    },

    center: {
        alignItems: 'center',
        justifyContent: 'center',
    },

    image: {
        width: 330,
        height: 280,
        borderRadius: 15,
        marginBottom: 20,
    },

    labelTitle: {
        fontSize: 14,
        fontWeight: 'bold',
        marginBottom: 3,
    },

    label: {
        fontSize: 18,
        fontWeight: 'bold',
        marginTop: 20,
        marginBottom: 10,
    },

    // starsBar: {
    //     width: 130,
    //     height: 30,
    //     flexDirection: 'row',
    //     justifyContent: 'space-between',
    //     alignItems: 'center',
    //     marginBottom: 15,
    //     backgroundColor: 'white',
    //     borderRadius: 15,
    //     paddingHorizontal: 10,
    // },

    // stars: {
    //     justifyContent: 'center',
    //     flexDirection: 'row',
    //     marginBottom: 2,
    //   },

    //   star: {
    //     fontSize: 18,
    //     marginRight: 5,
    //   },

    batteryContainer: {
        width: '100%',
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        paddingBottom: 20,
    },

    batteryBody: {
        width: 50,
        height: 20,
        marginLeft: 2,
        borderWidth: 2,
        borderColor: 'grey',
        borderRadius: 5,
        justifyContent: 'center',
        position: 'relative', // Added to position children absolutely
    },

    batteryHead: {
        width: 3,
        height: 15,
        backgroundColor: 'grey',
        borderRadius: 2,
    },

    batteryLevel: {
        height: '100%',
        borderRadius: 3,
        position: 'absolute', // Ensure it doesn't affect layout of sibling elements
        left: 0, // Align to the left side of the batteryBody
        top: 0, // Align to the top of the batteryBody
    },
    batteryText: {
        fontSize: 12,
        color: 'black',
        fontWeight: 'bold',
        position: 'absolute',
        alignSelf: 'center',
        top: '50%',
        transform: [{ translateY: -8 }],
    },


});

export default PlantProfile;
