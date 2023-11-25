import axios from 'axios';
import { useState } from 'react';

const client = axios.create({
    baseURL: 'http://localhost:3010/api'
});


export const defaultRequest = {
    url: '',
    method: '',
    headers: JSON.stringify({})
};

export function requestReducer(state, action) {
    var targetState={...state};
    switch (action.type) {
        case 'LOADCONFIG':
            targetState.url = '/config';
            targetState.method = 'get';
            //  defaultRequest.body = JSON.stringify(data);
            break;
        case 'setColor':
                targetState.url = '/color';
                targetState.method = 'put';
                targetState.data = action.body;
                targetState.params = action.params;
            //  defaultRequest.body = JSON.stringify(data);
            console.log(targetState);
            break;
        case "setBrightness":
            targetState.url = '/brightness';
            targetState.method = 'put';
            targetState.data = action.body;
            targetState.params = action.params;
        console.log(targetState);
        break; 
        case "calibrateLdr":
            targetState.url = '/calibrateLdr';  
            targetState.method = 'get';
        break;        
        default:
            break;
    }
    return targetState;
}

export default function useRequest(request) {
    const [response, setResponse] = useState(null);
    const [error, setError] = useState('');
    const handleRequest = async (request) => {

        try {
            var data = await client.request(request);
            setResponse(data);
        } catch (error) {
            setError(error.message);
        }

    }

    return [response, error, (request) => handleRequest(request)];
}