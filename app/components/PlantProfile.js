import React, { useState } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, TextInput, Switch, TouchableOpacity, StyleSheet, Platform, Keyboard, TouchableWithoutFeedback } from 'react-native';
// import StarRating from "react-svg-star-rating";

const PlantProfile = () => {
    // const [rating, setRating] = useState(0);    

    // const healthActiveColor = "#70A327";
    // const healthInactiveColor = rbga(112, 163, 39, 0.5);
    // const unhealthActiveColor = "#FF0120";
    // const unhealthInactiveColor= rbga(255, 1, 32, 0.5);


    return (
        // <View>
        //     <Text>Plant Profile</Text>



        //     <View>
        //         <Text>Plant Name</Text>
        //         <Text>Plant Image</Text>
        //         <Text>Plant Description</Text>
        //         <Text>Plant Watering Schedule</Text>
        //         <Text>Plant Last Watered</Text>
        //         <Text>Plant Next Watering</Text>
        //     </View>
        // </View>

        <KeyboardAvoidingView 
            behavior={Platform.OS === 'ios' ? 'padding' : "height"}
            style={styles.container}>
            <ScrollView>

                <TouchableWithoutFeedback onPress={Keyboard.dismiss}>
                    <View style={styles.inner}>
               
                    <View>
                        <Text style={styles.label}>Plant Watering System</Text>
                        
                    </View>


                    </View>
                </TouchableWithoutFeedback>
        </ScrollView>
    </KeyboardAvoidingView>
    );
}

const styles = StyleSheet.create({
    
});

export default PlantProfile;
